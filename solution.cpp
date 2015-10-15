
#include <iostream>
#include <vector>
#include <sstream>

//#include <set>
//#include <unordered_set>
//#include <algorithm>
//#include <cstring>


using namespace std;


typedef int INT;
typedef vector<INT> storage;






int cmp_impl(const INT* ring, int N, int beg1, int end1, int beg2, int end2/*, int size*/) {
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
        int tmp_n = N;
        int tmp_k = K;
        while (tmp_k) {
            int s = tmp_n % tmp_k ? 
                    tmp_n / tmp_k + 1:
                    tmp_n / tmp_k;
            tmp_k -= 1;
            tmp_n -= s;
            part_size_num += (s == part_size);
        }

        int min_beg = 0, min_end = 0;

        for (int b = 0; b < N; ++b) {
            int  e = (b + part_size) % N; // increment(b, N, part_size);

            if (min_beg != min_end) {
                if (ring[min_beg] < ring[b])
                    continue;
                else {
                    int res = cmp(ring, N, min_beg, min_end, b, e/*, part_size*/);
                    if (0 >= res)
                        continue;
                }
            }


            //begins[0] = b;
            //begins[1] = e;


            int cur_beg = 2;
            int full_parts = part_size_num - 1;

            int tmp_b = e;
            int tmp_e = (tmp_b + part_size) % N;
            int space = part_size;


            while ((space + part_size) <= N && full_parts) {
                int res = cmp(ring, N, b, e, tmp_b, tmp_e);

                if (0 <= res) {
                    full_parts -= 1;
                    space += part_size;
                    tmp_b = tmp_e;
                }
                else {
                    space += 1;
                    tmp_b = (tmp_b + 1) % N;
                }
                tmp_e = (tmp_b + part_size) % N;

            }   // while true
                
            // last one partition
            if (0 == full_parts) {
                min_beg = b;
                min_end = e; 
            }
               
        }


        stringstream ss;
    
        for (;min_beg != min_end; min_beg=(min_beg + 1) % N)
            ss << (int)ring[min_beg];
        cout << ss.str() << endl;
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
