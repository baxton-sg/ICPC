
#include <iostream>
#include <vector>
#include <set>
//#include <unordered_set>
#include <algorithm>

#include <cstring>


using namespace std;


typedef int INT;
typedef vector<INT> storage;






int cmp_impl(const INT* ring, int N, int beg1, int end1, int beg2, int end2, int size) {
    /*
    int res;
    int size4 = size / 4;
    for (int i = 0; i < size4; ++i) {
        res = ring[beg1] - ring[beg2];
        if (0 < res) return 1;
        else if (0 > res) return -1;
        beg1 = (beg1 + 1) % N;
        beg2 = (beg2 + 1) % N;    
     
        res = ring[beg1] - ring[beg2];
        if (0 < res) return 1;
        else if (0 > res) return -1;
        beg1 = (beg1 + 1) % N;
        beg2 = (beg2 + 1) % N;   
        
        res = ring[beg1] - ring[beg2];
        if (0 < res) return 1;
        else if (0 > res) return -1;
        beg1 = (beg1 + 1) % N;
        beg2 = (beg2 + 1) % N;   
        
        res = ring[beg1] - ring[beg2];
        if (0 < res) return 1;
        else if (0 > res) return -1;
        beg1 = (beg1 + 1) % N;
        beg2 = (beg2 + 1) % N;          
    }
    for (int i = size4*4; i < size; ++i) {
        res = ring[beg1] - ring[beg2];
        if (0 < res) return 1;
        else if (0 > res) return -1;
        beg1 = (beg1 + 1) % N;
        beg2 = (beg2 + 1) % N;           
    }
    return 0;
    */
    
    
    int res = ring[beg1] - ring[beg2];
    while(!res && beg1 != end1) {
        beg1 = (beg1 + 1) % N;
        beg2 = (beg2 + 1) % N;
        res = ring[beg1] - ring[beg2];
    }
    return res;
    
}



// 3 <= N && N <= 100000
// 2 <= K && K <= N
template<int cmp(const INT*, int, int, int, int, int, int)>
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
        int tmp_n = N;
        int tmp_k = K;
        while (tmp_k) {
            int s = tmp_n % tmp_k ? 
                    tmp_n / tmp_k :
                    tmp_n / tmp_k + 1;
            tmp_k -= 1;
            tmp_n -= s;
            part_size_num += (s == part_size);
        }

//        vector<int> begins(K, 0);
        int min_beg = 0, min_end = 0;
        int tmp_psn = part_size_num;

        for (int b = 0; b < N; ++b) {
            int  e = (b + part_size) % N; // increment(b, N, part_size);

            if (min_beg != min_end) {
                if (ring[min_beg] < ring[b])
                    continue;
                else {
                    int res = cmp(ring, N, min_beg, min_end, b, e, part_size);
                    if (0 >= res)
                        continue;
                }
            }


            //begins[0] = b;
            //begins[1] = e;


            int cur_beg = 2;
            int prev = e;
            int full_parts = part_size_num - 1;

//            process_next(ring, N, K, begins, 2, part_size, beg, end);
            while(K > cur_beg) {
        
                //int size_last = size(begins[cur_beg-1], begins[0], N);

                int size_last = b - prev;
                size_last += N * ((unsigned int)size_last >> (8 * sizeof(int) - 1));
                if (size_last <= part_size) {
                    min_beg = b;
                    min_end = e;
                    break;
                }        

                int tmp_b = prev;
                int tmp_e = (tmp_b + part_size) % N; //increment(b, N, part_size);
        
                int res = cmp(ring, N, b, e, tmp_b, tmp_e, part_size);
        
                //if (0 > res) {
                    // 1st smaller
        
                    //e = decrement(e, N, 1);
                    //--tmp_e;
                    int flag = ((unsigned int)res >> (8 * sizeof(int) - 1));
                    tmp_e -= flag;
                    tmp_e = tmp_e + N * ((unsigned int)tmp_e >> (8 * sizeof(int) - 1));
                    full_parts -= 1 - flag;

                
 //                   if (tmp_e == tmp_b)
 //                       break;
                //}
        
                prev = tmp_e;
        
                cur_beg += 1;
                
                if ((K - cur_beg + 1) < full_parts)
                    break;
            }   // while true
                
            // last one partition
            if (K == cur_beg) {
                //int size_last = size(begins[cur_beg-1], begins[0], N);
                int size_last = b - prev;
                size_last += N * ((unsigned int)size_last >> (8 * sizeof(int) - 1));
                if (size_last < part_size) {
                    min_beg = b;
                    min_end = e; 
                }
                else if (size_last == part_size) {
                    if (ring[b] < ring[prev])
                        continue;
                    else {
                        int res = cmp(ring, N, b, e, prev, b, part_size);
                        if (0 <= res) {
                            min_beg = b;
                            min_end = e; 
                        }
                    }
                }    
            }
               
        }


        for (;min_beg != min_end; min_beg=(min_beg + 1) % N)
            cout << (int)ring[min_beg];
        cout << endl;
    }

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
