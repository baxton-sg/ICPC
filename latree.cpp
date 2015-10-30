


#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>


using namespace std;



const int MAX_N = 20000;
const int KEY_SIZE = MAX_N - 1;


typedef char(key_t)[KEY_SIZE];




void init_key(key_t& k) {
    for (int i = 0; i < KEY_SIZE; ++i)
        k[i] = 0;
}

void add_letter(char c, key_t& dst, const key_t& src) {
    dst[KEY_SIZE-1] = c;
    for (int i = KEY_SIZE-2; i >= 0; --i)
        dst[i] = src[i+1];
}


int cmp_key(const key_t& k1, const key_t& k2) {
    for (int i = KEY_SIZE-1; i >= 0; --i) {
        if (k1[i] > k2[i])
            return 1;
        else if (k1[i] < k2[i])
            return -1;
        else if (k1[i] == 0 && k2[i] == 0)
            break;
    }

    return 0;
}



struct edge_t {
    int beg;
    int end;
    char c;

    edge_t(int b, int e, char ch) :
        beg(b),
        end(e),
        c(ch)
    {}
};






struct params {
    int N;

    vector<edge_t> edges;
    vector<int> edges_num;
};


void init_params(params& p) {
    edges.reserve(KEY_SIZE);
    edges_num.assign(p.N, 0);
}


void add_edge(params& p, int a, int b, char c) {
    --a;
    --b;
    p.edges_num[a] += 1;
    p.edges_num[b] += 1;
}





void solve(params& p) {


    cout << endl;
}


int main(int argc, const char* argv[]) {

    auto_ptr<params> pp(new params);
    params& p = *pp.get();

    cin >> p.N;

    init_params(p);

    for (int i = 0; i < p.N-1; ++i) {
        int a, b;
        char c;
        cin >> a >> b >> c;
        add_edge(p, a, b, c);
    }


    solve(p);


    key_t k;
    init_key(k);
    int idx = KEY_SIZE-1;
    k[idx--] = 'm';
    k[idx--] = 'a';
    k[idx--] = 'x';

    char* b = upper_bound(&k[0], &k[KEY_SIZE], char(0));
    cout << *b << "; " << *(b-1) << endl;


    return 0;
}
