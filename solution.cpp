

#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;





inline
int advance(int it, int size) {
    return (it + 1) % size;
}

inline
int size(int beg, int end, int N) {
    return (beg <= end ? end - beg : N - beg + end);
}


int max(const vector<int>& ring, int size) {
    int m = ring[0];
    for (int i = 1; i < size; ++i)
        m = m < ring[i] ? ring[i] : m;
    return m;
}




int cmp(const vector<int>& ring, int N, int beg1, int end1, int beg2, int end2) {
    while (beg1 != end1 && 0 == ring[beg1])
        beg1 = advance(beg1, N);
    while (beg2 != end2 && 0 == ring[beg2])
        beg2 = advance(beg2, N);

    int size1 = size(beg1, end1, N);
    int size2 = size(beg2, end2, N);

    if (size1 > size2) {
        return 1;
    }
    else if (size2 > size1) {
        return -1;
    }

    // sizes are equal       
    while (beg1 != end1 && beg2 != end2) {
        if (ring[beg1] > ring[beg2])
            return 1;
        else if (ring[beg2] > ring[beg1])
            return -1;
        beg1 = advance(beg1, N);
        beg2 = advance(beg2, N);
    }
    
    return 0; 
}

bool iterate_parts(const vector<int>& ring, int N, int K, vector<int>& begins, int cur_beg, int& min_beg, int& min_end) {

    for (int b = advance(begins[cur_beg-1], N); b != begins[0]; b = advance(b, N)) {

        begins[cur_beg] = b;

        if (1 == cur_beg) {
            if (min_beg != min_end) {
                // new 1st partition, check if it's smaller than one I already have
                int res = cmp(ring, N, min_beg, min_end, begins[0], begins[1]);
                if (1 != res) 
                    // I do not need to continue bcoz 1st partition will be only getting bigger
                    return false;
            }
        }
        else {
            // check if the partition is smaller than or equal to the 1st one
            int res = cmp(ring, N, begins[0], begins[1], begins[cur_beg-1], begins[cur_beg]);
            if (-1 == res)
                // I do not need to continue bcoz the current partition will be only getting bigger
                return false;
        }

        if ((K-1) == cur_beg) {
            int res = cmp(ring, N, begins[0], begins[1], begins[cur_beg], begins[0]);
            if (-1 != res) {
                // last partition - I'm here and this means I found one of solutions
                min_beg = begins[0];
                min_end = begins[1];
                return true; 
            }
        }
        else {
            int pos_available = size(begins[cur_beg], begins[0], N) - 1;
            if (pos_available < (K - cur_beg - 1))
                return false; 

            iterate_parts(ring, N, K, begins, cur_beg+1, min_beg, min_end);
        }
    }


    return false;
}




// 3 <= N && N <= 100000
// 2 <= K && K <= N
void solve(const vector<int>& ring, int N, int K) {
    if (K == N)
        cout << max(ring, N) << endl;
    else {
        vector<int> begins(K, 0);
        int beg = 0, end = 0;

        for (int i = 0; i < N; ++i) {
            int b = i;

            if (b && ring[b-1] == 0)
                continue;

            begins[0] = b;
            iterate_parts(ring, N, K, begins, 1, beg, end);
        }


        if (beg == end)
            cout << "0" << endl;
        else {
            while (beg != end && 0 == ring[beg])
                beg = advance(beg, N);
            for (;beg != end; beg=advance(beg, N))
                cout << ring[beg];
            cout << endl;
        }
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
