






#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
//#include <cmath>
//#include <cstring>
//#include <memory>



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

    INT ring[100000];

};

inline 
int cmp(const params& p, int beg1, int beg2) {

    if (p.part_size <= sizeof(INT)) {
        if (p.ring[beg1] > p.ring[beg2])
            return 1;
        else if (p.ring[beg1] < p.ring[beg2])
            return -1;
        return 0;
    }

    int size = p.part_size % p.num_per_INT ? p.part_size / p.num_per_INT - 1 : p.part_size / p.num_per_INT; //sizeof(INT);


    for (int i = 0; i < size; ++i) {
        if (p.ring[beg1] > p.ring[beg2])
            return 1;
        else if (p.ring[beg1] < p.ring[beg2])
            return -1;

        beg1 = (beg1 + p.num_per_INT) % p.N;
        beg2 = (beg2 + p.num_per_INT) % p.N;
    } 

    if ((p.ring[beg1] & p.last_mask) > (p.ring[beg2] & p.last_mask))
        return 1;
    else if ((p.ring[beg1] & p.last_mask) < (p.ring[beg2] & p.last_mask))
        return -1;

    return 0;
}


void countSort(const params& p, int* __restrict__ indices, int dig_num) {
    int output[p.N];
    int count[10] = {0};
    
    int shift = p.part_size <= sizeof(INT) ? (p.part_size - 1) * 8 : (sizeof(INT) - 1) * 8;
    

    for (int i = 0; i < p.N; ++i) {
        INT val = (p.ring[(indices[i] + dig_num)%p.N] & p.first_mask) >> shift;
        count[val] += 1;
    }

    for (int i = 1; i < 10; ++i) 
        count[i] += count[i-1];

    for (int i = p.N - 1; i >= 0; --i) {
        INT val = (p.ring[(indices[i] + dig_num)%p.N] & p.first_mask) >> shift;
        output[ count[val] - 1 ] = indices[i];
        count[val] -= 1;
    }

    for (int i = 0; i < p.N; ++i)
        indices[i] = output[i];
}

void radixSort(const params& p, int* __restrict__ indices) {
    int output[p.N];
    int count[10];

    for (int s = 0; s < p.part_size; ++s) {
        int dig_num = p.part_size - s - 1;
        
        count[0] = 0;
        count[1] = 0;
        count[2] = 0;
        count[3] = 0;
        count[4] = 0;
        count[5] = 0;
        count[6] = 0;
        count[7] = 0;
        count[8] = 0;
        count[9] = 0;

        int shift = p.part_size <= sizeof(INT) ? (p.part_size - 1) * 8 : (sizeof(INT) - 1) * 8;


        for (int i = 0; i < p.N; ++i) {
            INT val = (p.ring[(indices[i] + dig_num)%p.N] & p.first_mask) >> shift;
            count[val] += 1;
        }

        for (int i = 1; i < 10; ++i) 
            count[i] += count[i-1];

        for (int i = p.N - 1; i >= 0; --i) {
            INT val = (p.ring[(indices[i] + dig_num)%p.N] & p.first_mask) >> shift;
            output[ count[val] - 1 ] = indices[i];
            count[val] -= 1;
        }

        for (int i = 0; i < p.N; ++i)
            indices[i] = output[i];
        
        
    }
        //countSort(p, indices, p.part_size - s - 1);
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
    return os;
}




int count_num(const params& p, set<int>& tail, int* indices, int start) {
    int accum = 0;

    if (start) {

        const int end1 = indices[start] + p.part_size - 1;
        const int end2 = indices[start] + p.N - p.part_size + 1;

        // counting
        bool found_small = false;
        set<int>::const_iterator is = tail.begin();
        int is_val;
        while (!found_small && is != tail.end()) {
            is_val = *is > indices[start] ? *is : *is + p.N;

            if (end1 < is_val && is_val < end2)
                found_small = true;
            else
                ++is;
        }

        if (found_small) {
            set<int>::const_iterator ib = is; ++ib;

            for (; ib != tail.end(); ++ib) {
                int tmp = *ib > indices[start] ? *ib : *ib + p.N;
                if (end1 < tmp && tmp < end2) {
                    const int space = tmp > is_val ? tmp - is_val : is_val - tmp;
                    if (space >= p.part_size) {
                        ++accum;
                        is = ib;
                        is_val = *is > indices[start] ? *is : *is + p.N;
                    }
                }
            }
            ++accum;
        }

    }

    return accum;
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


        int indices[p.N];
        for (int i = 0; i < p.N; ++i)
            indices[i] = i;
        
        if (15 < p.part_size)
            sort(&indices[0], &indices[p.N], idx_less(p));
        else
            radixSort(p, indices);


        if (p.part_size_num == 1) {
            stringstream ss;
            int min_beg = indices[0];
            int min_end = (min_beg + p.part_size) % p.N;
            print(ss, p, min_beg, min_end);
            cout << ss.str();
            return;
        }


        
        int back_indices[p.N * 2] __attribute__((aligned(16)));
        int size4 = p.N / 4 * 4;
        for (int i = 0; i < size4; i += 4) {
            back_indices[indices[i]] = i;
            back_indices[indices[i+1]] = i+1;
            back_indices[indices[i+2]] = i+2;
            back_indices[indices[i+3]] = i+3;
        }
        for (int i = size4; i < p.N; ++i)
            back_indices[indices[i]] = i;

        for (int i = p.N; i < p.N * 2; ++i)
            back_indices[i] = back_indices[i - p.N]; 
 

        int accum = 0;
        int start = p.part_size_num - 1;
        set<int> tail;

        // initial population
        if (start) {
            for (int i = 0; i < start; ++i) {
                tail.insert(indices[i]);
            }

            // counting
            accum = count_num(p, tail, indices, start);

        }

        while (accum < (p.part_size_num - 1)) {

            int i = 0;
            int step = p.N - 1 - start > 500 ? 500 : 1;
            while (i++ < step && start < (p.N-1))
               tail.insert(indices[start++]);

            accum = count_num(p, tail, indices, start);
        }

	while (accum > (p.part_size_num - 1)) {
            set<int>::iterator it = tail.find(indices[--start]);
            tail.erase(it);
            accum = count_num(p, tail, indices, start);
	}


    

        int min_beg = 0, 
            min_end = 0;

        for (int i = start; i < p.N; ++i) {
            int b = indices[i];
            int b_size = back_indices[b];
            
            int full_parts = p.part_size_num - 1;
            int full_parts_small = p.part_size_small_num;

            int tmp_b = b + p.part_size;

            while (full_parts) {

                int res = b_size - back_indices[tmp_b];

                if (0 <= res) {
                    full_parts -= 1;
                    tmp_b = tmp_b + p.part_size;
                }
                else {
                    if (0 == full_parts_small)
                        break;
                    full_parts_small -= 1;
                    tmp_b = tmp_b + (p.part_size - 1);
                }            

            }   // while true
                
            // last one partition
            if (0 == full_parts) {
                min_beg = b;
                min_end = (b + p.part_size) % p.N;; 
                break;
            }
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
    INT tmp_mask = p.part_size <= sizeof(INT) ? ~((~INT(0) >> shift) << shift) : ~INT(0);
    shift = (sizeof(INT) - p.part_size % sizeof(INT)) * 8;
   
    p.last_mask = (tmp_mask >> shift) << shift;

    shift = p.part_size <= sizeof(INT) ? (p.part_size - 1) * 8 : (sizeof(INT) - 1) * 8;
    p.first_mask = INT(0xFF) << shift; 


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


    return 0;
}
