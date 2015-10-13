

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>


using namespace std;





inline
int advance(int it, int size, int step=1) {
    if (0 > step) {
        int s = it - (-step) % size;
        return s >= 0 ? s : size + s;
    }
    
    return (it + step) % size;
}

inline
int size(int beg, int end, int N) {
    return (beg <= end ? end - beg : N - beg + end);
}






int cmp(const vector<int>& ring, int N, int beg1, int end1, int beg2, int end2) {

    // sizes are equal !!!      
    while (beg1 != end1) {
        if (ring[beg1] > ring[beg2])
            return 1;
        else if (ring[beg2] > ring[beg1])
            return -1;
        beg1 = advance(beg1, N);
        beg2 = advance(beg2, N);
    }
    
    return 0; 
}



struct num_less {
    const vector<int>& ring_;
    int N_;
    num_less(const vector<int>& ring, int N) :
        ring_(ring),
        N_(N)
    {}


    bool operator() (const pair<int,int>& p1, const pair<int,int>& p2) {
        return 0 > cmp(ring_, N_, p1.first, p1.second, p2.first, p2.second);
    }
};



bool get_part(const vector<int>& ring, int N, int b1, int e1, int start, int finish, int part_size, int& beg, int& end) {
    // sanity check
    if (part_size > size(start, finish, N))
        return false;

    int stop = advance(finish, N, -part_size + 1);

    int min_beg = start;
    int min_end = -1;
    for (;min_beg != stop; min_beg = advance(min_beg, N)) { 
        min_end = advance(min_beg, N, part_size);
        int res = cmp(ring, N, b1, e1, min_beg, min_end);
        if (0 <= res) {
            beg = min_beg;
            end = min_end;
            return true;
        }
    }

    return false;
}


// 3 <= N && N <= 100000
// 2 <= K && K <= N
void solve(const vector<int>& ring, int N, int K) {
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

        int part_size = (N % K) ? (N / K) + 1 : (N / K);
        int part_size_num = 0;

        int tmp_N = N;
        int tmp_K = K;
        while (tmp_N) {
            int s = (tmp_N % tmp_K) ? (tmp_N / tmp_K) + 1 : (tmp_N / tmp_K);
            if (s == part_size)
                part_size_num += 1;

            tmp_N -= s;
            tmp_K -= 1;
        }

        int min_beg = 0, min_end = 0;
        int res;

        for (int b = 0; b < N; ++b) {
            int e = advance(b, N, part_size);

            if (min_beg != min_end) {
                res = cmp(ring, N, min_beg, min_end, b, e);
                if (0 >= res)
                    continue;
            }

            int tmp_num = part_size_num - 1;
            int beg = e, end = b;
            while(tmp_num) {
                if (!get_part(ring, N, b, e, beg, end, part_size, beg, end))
                    break;
                --tmp_num;
                beg = end;
                end = b;
            }

            if (!tmp_num) {
                min_beg = b;
                min_end = e;
            }
        }


        for (;min_beg != min_end; min_beg=advance(min_beg, N))
            cout << ring[min_beg];
        cout << endl;
    }
}




int main(int argc, const char* argv[]) {

    int N, K;
    cin >> N >> K;

    vector<int> ring(N, 0);

    for (int i = 0; i < N; ++i) {
        char c;
        cin >> c;
        ring[i] = int(c - '0');
    }

    solve(ring, N, K);


    return 0;
}
