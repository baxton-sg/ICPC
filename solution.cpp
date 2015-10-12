

#include <iostream>
#include <vector>
#include <algorithm>


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


int try_bigger(const vector<int>& ring, int N, int K, vector<int>& begins, int cur_beg, int b) {
    int try_b = advance(b, N);

    while (try_b != begins[0]) {

        int res = cmp(ring, N, begins[0], begins[1], begins[cur_beg-1], try_b);

        if (0 > res) 
            break;

        b = try_b;
        try_b = advance(try_b, N);
    }

    return b;
}


int try_smaller(const vector<int>& ring, int N, int K, vector<int>& begins, int cur_beg, int b) {
    int try_b = advance(b, N, -1);

    while (try_b != begins[cur_beg-1]) {

        int res = cmp(ring, N, begins[0], begins[1], begins[cur_beg-1], try_b);
    
        if (0 <= res) 
            break;

        try_b = advance(try_b, N, -1);
    }

    return try_b;
}



bool process_next(const vector<int>& ring, int N, int K, vector<int>& begins, int cur_beg, int size_1st, int& min_beg, int& min_end) {

    if (K == cur_beg) {
        // last one partition

        int res = cmp(ring, N, begins[0], begins[1], begins[cur_beg-1], begins[0]);

        if (-1 != res) {
            min_beg = begins[0];
            min_end = begins[1]; 
            return true;
        }
        
        return false;
    }


    int size_last = size(begins[cur_beg-1], begins[0], N);
    int pos_available = size_last ? size_last - 1 : 0;
    if (pos_available < (K - cur_beg)) {
        int beg = cur_beg-1;
        int res = cmp(ring, N, begins[0], begins[1], begins[beg], begins[0]);

        if (-1 != res) {
            min_beg = begins[0];
            min_end = begins[1];
            return true;
        }
        
        return false;
    }


    int step = size_1st > pos_available ? pos_available : size_1st;
    int b = advance(begins[cur_beg-1], N, step);

    int res = cmp(ring, N, begins[0], begins[1], begins[cur_beg-1], b);

    if (0 <= res) {
        // 1st bigger

        b = try_bigger(ring, N, K, begins, cur_beg, b);
    }
    else if (0 > res) {
        // 1st smaller

        b = try_smaller(ring, N, K, begins, cur_beg, b);
        if (b == begins[cur_beg-1])
            return false;
    }

    begins[cur_beg] = b;

    return process_next(ring, N, K, begins, cur_beg+1, size_1st, min_beg, min_end);
}


void set_2nd(const vector<int>& ring, int N, int K, vector<int>& begins, int& min_beg, int& min_end) {
    int size_1st = 0;
   
    for (int b = advance(begins[0], N); b != begins[0]; b = advance(b, N)) { 
        ++size_1st;

        begins[1] = b;

        if (K == 2) {
            if (min_beg != min_end) {
                int res = cmp(ring, N, begins[0], begins[1], min_beg, min_end);
                if (-1 != res)
                    return;
            }
       
            int res = cmp(ring, N, begins[1], begins[0], begins[0], begins[1]); 
            if (1 == res)
                continue;

            min_beg = begins[0];
            min_end = begins[1];
            return;
        }
        else {
            if (min_beg != min_end) {
                int res = cmp(ring, N, min_beg, min_end, begins[0], begins[1]);
                if (1 != res)
                    return;
            }

            bool result = process_next(ring, N, K, begins, 2, size_1st, min_beg, min_end);
            if (result)
                return;
        }
    }
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
        vector<int> begins(K, 0);
        int beg = 0, end = 0;

	int step = 1;

        for (int i = 0; i < N; i+=step) {
            int b = i;

            begins[0] = b;
            set_2nd(ring, N, K, begins, beg, end);

            if (beg != end) {
                int size_1st = size(beg, end, N);
                step = size_1st / 2;
            }
        }


        if (beg == end)
            cout << "0" << endl;
        else {
            while (beg != end && 0 == ring[beg])
                beg = advance(beg, N);

            if (beg == end)
                cout << "0" << endl;
            else {
                for (;beg != end; beg=advance(beg, N))
                    cout << ring[beg];
                cout << endl;
            }
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
