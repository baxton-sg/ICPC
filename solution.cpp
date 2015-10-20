
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>
#include <memory>


using namespace std;


typedef int INT;
typedef vector<INT> storage;






int cmp_impl(const INT* ring, int N, int beg1, int end1, int beg2, int end2/*, int size*/) {

    int res = 0;
    while(!res && beg1 != end1) {
        res = ring[beg1] - ring[beg2];
        beg1 = (beg1 + 1) % N;
        beg2 = (beg2 + 1) % N;
    }
    return res;
    
}



struct idx_less {
    const INT* ring;
    int size;
    int N;
    idx_less(const INT* v, int v_size, int part_size) :
        ring(v),
        size(part_size),
        N(v_size)
    {}

    bool operator() (int i1, int i2) {
        int e1 = (i1 + size) % N;
        int e2 = (i2 + size) % N;
        int res = cmp_impl(ring, N, i1, e1, i2, e2);
        return 0 > res;
    }
};



void countSort(const INT* ring, int N, int part_size, vector<int>& indices, int dig_num) {
    auto_ptr<int> output(new int[N]);
    int count[10] = {0};
    

    for (int i = 0; i < N; ++i) {
        INT val = ring[(indices[i] + dig_num)%N] % 10;
        count[val] += 1;
    }

    for (int i = 1; i < 10; ++i) 
        count[i] += count[i-1];

    for (int i = N - 1; i >= 0; --i) {
        INT val = ring[(indices[i] + dig_num)%N] % 10;
        output.get()[ count[val] - 1 ] = indices[i];
        count[val] -= 1;
    }

    for (int i = 0; i < N; ++i)
        indices[i] = output.get()[i];
}

void radixSort(const INT* ring, int N, int part_size, vector<int>& indices) {
    for (int s = 0; s < part_size; ++s) 
        countSort(ring, N, part_size, indices, part_size - s - 1);
}


// 3 <= N && N <= 100000
// 2 <= K && K <= N
template<int cmp(const INT*, int, int, int, int, int/*, int*/)>
void solve(const INT* ring, int N, int K) {
    if (K == N) {
        int m = ring[0];
        if (m < 9) {
            for (int i = 0; i < N; ++i) {
                if (m < ring[i]) {
                    m = ring[i];
                    if (m == 9)
                        break;
                } 
            }
        }
        cout << m << endl;
    }
    else {

        int part_size = N / K;
        part_size = (N % K) ?  part_size + 1 : part_size;
        int part_size_num = 0;
        int part_size_small = 0;
        int tmp_n = N;
        int tmp_k = K;
        while (tmp_k) {
            int s = tmp_n % tmp_k ? 
                    tmp_n / tmp_k + 1:
                    tmp_n / tmp_k;
            tmp_k -= 1;
            tmp_n -= s;
            part_size_num += (s == part_size);
            part_size_small += (s != part_size);
        }



        vector<int> indices;
        indices.reserve(N);
        for (int i = 0; i < N; ++i)
            indices.push_back(i);
        //sort(indices.begin(), indices.end(), idx_less(ring, N, part_size));
        radixSort(ring, N, part_size, indices);

        vector<int> back_indices(N, 0);
        back_indices.reserve(N);
        for (int i = 0; i < N; ++i) {
            back_indices[indices[i]] = i;
        }

/*
        multiset<int, idx_less> indices(idx_less(ring, N, part_size));
        for (int i = 0; i < N; ++i)
            indices.insert(i);
*/


/*
for (int i = 0; i < N; ++i) {
    int b = indices[i];
    int e = (b + part_size) % N;
    cout << i << " [" << indices[i] << "] ";
    for (int j = b; j != e; j = (j+1)%N)
        cout << ring[j];
    cout << endl;
}
*/

        int min_beg = 0, min_end = 0;

        for (int i = 0; i < N; ++i) {
        //for (multiset<int, idx_less>::const_iterator it = indices.begin(); it != indices.end(); ++it) {
            int b = indices[i];
//            int b = *it;
//                b = i;
            int  e = (b + part_size) % N; // increment(b, N, part_size);

//timer tm1("main loop");



//            if (min_beg != min_end) {
//                if (ring[min_beg] < ring[b])
//                    continue;
//                else {
//                    int res = cmp(ring, N, min_beg, min_end, b, e/*, part_size*/);
//                    if (0 >= res)
//                        continue;
//                }
//            }


            //begins[0] = b;
            //begins[1] = e;


            int full_parts = part_size_num - 1;
            int full_parts_small = part_size_small;

            int tmp_b = e;
            int tmp_e = (tmp_b + part_size) % N;
            int space = part_size;


            while (full_parts) {
//timer tm2("nested loop");

                //int res = cmp(ring, N, b, e, tmp_b, tmp_e);
                int res = back_indices[b] - back_indices[tmp_b];

                if (0 <= res) {
                    full_parts -= 1;
                    space += part_size;
                    tmp_b = tmp_e;
                }
                else {
                    if (0 == full_parts_small)
                        break;
                    space += part_size - 1;
                    full_parts_small -= 1;
                    tmp_b = (tmp_b + (part_size - 1)) % N;
                }
                tmp_e = (tmp_b + part_size) % N;

            }   // while true
                
            // last one partition
            if (0 == full_parts) {
/*
cout << "found: ";
for (int z = b; z != e; z = (z + 1) % N)
    cout << ring[z];
cout << endl;
*/
                min_beg = b;
                min_end = e; 
                break;
            }
               
        }


        stringstream ss;
    
        for (;min_beg != min_end; min_beg=(min_beg + 1) % N)
            ss << (int)ring[min_beg];
        cout << ss.str() << endl;
    }

//timer::print(cout);
}




int main(int argc, const char* argv[]) {

    int N, K;
    cin >> N >> K;

    storage ring(N+16, 0);
    size_t addr = 0;
    while((size_t)&ring[addr] % 16)
        addr += 1;
    
    string line;
    cin >> line;

    for (int i = addr; i < (N+addr); ++i) {
        ring[i] = int(line[i-addr] - '0');
    }

    solve<cmp_impl>(&ring[addr], N, K);


    return 0;
}
