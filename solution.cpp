

#include <iostream>
#include <vector>


using namespace std;


int N, K;



inline
int advance(int it, int size) {
    return (it + 1) % size;
}


int max(const vector<int>& ring, int size) {
    int m = ring[0];
    for (int i = 1; i < size; ++i)
        m = m < ring[i] ? ring[i] : m;
    return m;
}




int cmp(const vector<int>& ring, int beg1, int end1, int beg2, int end2) {
    while (beg1 != end1 && 0 == ring[beg1])
        beg1 = advance(beg1, N);
    while (beg2 != end2 && 0 == ring[beg2])
        beg2 = advance(beg2, N);

    int size1 = end1 - beg1;
    int size2 = end2 - beg2;

    if (size1 > size2) {
        return 1;
    }
    else if (size2 > size1) {
        return -1;
    }
       
    while (beg1 != end1 && beg2 != end2) {
        if (ring[beg1] > ring[beg2])
            return 1;
        else if (ring[beg2] > ring[beg1])
            return -1;
        beg1 = advance(beg1, N);
        beg2 = advance(beg2, N);
    }
    
    size1 = end1 - beg1;
    size2 = end2 - beg2;

    if (size1 > size2)
        return 1;
    else if (size2 > size1)
        return -1;

    return 0; 
}

bool iterate_parts(const vector<int>& ring, int N, int K, vector<int>& begins, int cur_beg, int& min_beg, int& min_end) {

    if (cur_beg == K)
        return false;
    


    for (int b = advance(begins[cur_beg-1], N); b != begins[0]; b = advance(b, N)) {
        begins[cur_beg] = b;

    }


    return true;
}


// 3 <= N && N <= 100000
// 2 <= K && K <= N
void solve(const vector<int>& ring) {
    if (K == N)
        cout << max(ring, N) << endl;
    else {
        vector<int> begins(K, 0);
        int beg = 0, end = 0;

        for (int b = 0; b < N; ++b) {
            begins[0] = b;
            iterate_parts(ring, N, K, begins, 1, beg, end);
        }

        for (;beg != end; beg=advance(beg, N))
            cout << ring[beg];
        cout << endl;
    }
}




int main(int argc, const char* argv[]) {


    cin >> N >> K;

    vector<int> ring(N, 0);

    for (int i = 0; i < N; ++i) {
        char c;
        cin >> c;
        ring[i] = int(c - '0');
    }

    solve(ring);


    return 0;
}
