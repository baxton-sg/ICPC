


#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>


using namespace std;


typedef unsigned int INT;





struct params {
    int N;
    int K;
    
    int part_size;
    int part_size_num;
    int part_size_small_num;

    int num_per_INT;
    INT last_mask;
    INT first_mask;    

    INT* ring;

    ~params() {
        delete [] ring;
    }
};


int cmp(const params& p, int beg1, int beg2) {

    if (p.part_size <= sizeof(INT)) {
        return p.ring[beg1] - p.ring[beg2];
    }

    int size = p.part_size / sizeof(INT);
    bool last_exists = p.part_size % sizeof(INT);

    for (int i = 0; i < size; ++i) {
        int res = p.ring[beg1] - p.ring[beg2];
        if (0 != res)
            return res;

        beg1 = (beg1 + p.num_per_INT) % p.N;
        beg2 = (beg2 + p.num_per_INT) % p.N;
    } 

    if (last_exists) 
        return (p.ring[beg1] & p.last_mask) - (p.ring[beg2] & p.last_mask); 

    return 0;
}



struct idx_less {
    const params& p;
    idx_less(const params& par) :
        p(par)
    {}

    bool operator() (int i1, int i2) {
        return 0 > cmp(p, i1, i2);
    }
};



ostream& print(ostream& os, const params& p, int b, int e) {
    int shift = p.part_size <= sizeof(INT) ? (p.part_size - 1) * 8 : (sizeof(INT) - 1) * 8;
    for (int z = b; z != e; z = (z + 1) % p.N) 
        cout << (INT)((p.ring[z] & p.first_mask) >> shift);
    cout << endl;
}



// 3 <= N && N <= 100000
// 2 <= K && K <= N
void solve(params& p) {
    if (p.K == p.N) {
        int m = p.ring[0];
        if (m < 9) {
            for (int i = 0; i < p.N; ++i) {
                if (m < p.ring[i]) {
                    m = p.ring[i];
                    if (m == 9)
                        break;
                } 
            }
        }
        cout << m << endl;
    }
    else {

/*
for (int i = 0; i < p.N; ++i) {
    cout << "[" << i << "] ";
    for (int j = sizeof(INT) - 1; j >= 0; --j) {
        INT v = (INT)0x00FF & (p.ring[i] >> (j * 8));
        cout << v;
    }
    cout << endl;
}
cout << "---" << endl;
*/


        vector<int> indices;
        indices.reserve(p.N);
        for (int i = 0; i < p.N; ++i) {
            indices.push_back(i);
        }
        sort(indices.begin(), indices.end(), idx_less(p));

/*
for (int z = 0; z < p.N; ++z) {
    cout << z << "\t[" << indices[z] << "]\t";
    int b = indices[z];
    int e = (b + p.part_size) % p.N;
    print(cout, p, b, e);
}
cout << endl;
*/


        int min_beg = 0, 
            min_end = 0;

        for (int i = 0; i < p.N; ++i) {
            int b = indices[i];
            int e = (b + p.part_size) % p.N;

            int full_parts = p.part_size_num - 1;
            int full_parts_small = p.part_size_small_num;

            int tmp_b = e;
            int tmp_e = (tmp_b + p.part_size) % p.N;


            while (full_parts) {

                int res = cmp(p, b, tmp_b);

                if (0 <= res) {
                    full_parts -= 1;
                    tmp_b = tmp_e;
                }
                else {
                    if (0 == full_parts_small)
                        break;
                    full_parts_small -= 1;
                    tmp_b = (tmp_b + (p.part_size - 1)) % p.N;
                }
                tmp_e = (tmp_b + p.part_size) % p.N;

            }   // while true
                
            // last one partition
            if (0 == full_parts) {
                min_beg = b;
                min_end = e; 
                break;

//                cout << "FOUND " << i << " [" << indices[i] << "] ";
//                print(cout, p, b, e);
            }
//            else {
//                cout << "not found " << i << " [" << indices[i] << "] ";
//                print(cout, p, b, e);
//            }
        }


        stringstream ss;
        print(ss, p, min_beg, min_end);
        cout << ss.str();
    }

}



void prepare_params(params& p) {
    p.part_size = p.N / p.K;
    p.part_size = (p.N % p.K) ?  p.part_size + 1 : p.part_size;
    p.part_size_num = 0;
    p.part_size_small_num = 0;
    int tmp_n = p.N;
    int tmp_k = p.K;
    while (tmp_k) {
        int s = tmp_n % tmp_k ?
                tmp_n / tmp_k + 1:
                tmp_n / tmp_k;
        tmp_k -= 1;
        tmp_n -= s;
        p.part_size_num += (s == p.part_size);
        p.part_size_small_num += (s != p.part_size);
    }

    p.num_per_INT = p.N > sizeof(INT) ? sizeof(INT) : p.N;
    p.num_per_INT = p.part_size > p.num_per_INT ? p.num_per_INT : p.part_size;

    int shift = sizeof(INT) - p.part_size;
    INT tmp_mask = p.part_size <= sizeof(INT) ? ~((~0 >> shift) << shift) : ~0;
    shift = (sizeof(INT) - p.part_size % sizeof(INT)) * 8;
   
    p.last_mask = (tmp_mask >> shift) << shift;

    shift = p.part_size <= sizeof(INT) ? (p.part_size - 1) * 8 : (sizeof(INT) - 1) * 8;
    p.first_mask = 0x00FF << shift; 

    p.ring = new INT[p.N];
    for (int i = 0; i < p.N; ++i)
        p.ring[i] = 0;
    
}

void push_val(params& p, INT val, int idx) {
    idx -= p.num_per_INT - 1;
    idx = idx + p.N * ((unsigned int)idx >> (8 * sizeof(int) - 1));

    for (int i = 0; i < p.num_per_INT; ++i) {
        p.ring[idx] |= INT(val) << (i * 8);

        idx = (idx + 1) % p.N;
    }
}


int main(int argc, const char* argv[]) {

    int N, K;
    cin >> N >> K;

    params p;
    p.N = N;
    p.K = K;
    prepare_params(p);


    string line;
    cin >> line;

    for (int i = 0; i < N; ++i) {
        push_val(p, INT(line[i] - '0'), i);
    }

    solve(p);
/*
    for (int i = 0; i < p.N; ++i) {
        cout << "[" << i << "] ";
        for (int j = sizeof(INT) - 1; j >= 0; --j) {
            INT v = (INT)0x00FF & (p.ring[i] >> (j * 8));
            cout << v;
        }
        cout << endl;
    }
    cout << "Last mask: ";
    for (int j = sizeof(INT) - 1; j >= 0; --j) {
        INT v = (INT)0x00FF & (p.last_mask >> (j * 8));
        cout << v;
    }
    cout << endl;
*/


    return 0;
}

