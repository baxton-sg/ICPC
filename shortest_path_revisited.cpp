


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;



const int INF = -1;




class Tri {

public:

    struct tri_node_t {
        tri_node_t* letters[26];
        int count;

        tri_node_t() :
            count(0) 
        {
            for (int i = 0; i < 26; ++i)
                letters[i] = NULL;
        }

        ~tri_node_t() {
            for (int i = 0; i < 26; ++i)
                if (letters[i]) 
                    delete letters[i];
        }
    };



    Tri() :
        head(new tri_node_t)
    {}

    ~Tri() {
        if (head)
            delete head;
    }


    void add_string(const char* beg, const char* end) {
        add_string_rec(head, beg, end);
    }

    const tri_node_t* get_next(const tri_node_t* cur, char c) const {
        if (!cur)
            cur = head;

        return cur->letters[c - 'a'];
    }


#if defined DEBUG
    ostream& print(ostream& os) {
        string tmp = "";
        return print_rec(os, head, tmp);
    }
#endif    


private:

#if defined DEBUG
    ostream& print_rec(ostream& os, const tri_node_t* node, string accum) {
        for (int i = 0; i < 26; ++i) {
            if (node->letters[i]) {
                os << accum << (char)(i + 'a') << " " << node->letters[i]->count << endl;
                string tmp_accum = accum + (char)(i + 'a');
                print_rec(os, node->letters[i], tmp_accum);
            }
        }
        return os;
    }
#endif


    tri_node_t* add_string_rec(tri_node_t* node, const char* beg, const char* end) {
        if (beg == end) 
            return NULL;
        

        tri_node_t* next = node->letters[*beg - 'a'];
        if (!next) {
            next = new tri_node_t;
            node->letters[*beg - 'a'] = next;
        }

        tri_node_t* res = add_string_rec(next, beg+1, end);
        if (!res) {
            next->count += 1;
            return next; 
        }
     
        return res;
    }






    tri_node_t* head;

};







struct params {
    string s;
    Tri subs;

    int N;
    int M;

    vector<int> graph;
    vector<int> dist;
    vector<char> labels;
    vector<const Tri::tri_node_t*> path_data;

    vector<int> heap;
};


struct node_less {
    const params& p_;
    node_less(const params& p) :
        p_(p)
    {}

    bool operator() (int i1, int i2) {
        bool res = false;

        if (p_.dist[i1] == INF && p_.dist[i2] == INF)
            res = false;
        else if (p_.dist[i1] == INF)
            res = false;
        else if (p_.dist[i2] == INF)
            res = true;
        else 
            res = p_.dist[i1] < p_.dist[i2];

        return !res; // for min heap
    }
};




void get_substrings(params& p) {
    int size = p.s.length();
    const char* str = p.s.c_str();

    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size+1; ++j) {
            p.subs.add_string(&str[i], &str[j]);
        }
    }
}


void init_graph(params& p) {
    p.graph.assign(p.N * p.N, 0);

    p.dist.assign(p.N, INF);
    p.dist[0] = 0;

    p.labels.assign(p.N * p.N, 0);

    p.path_data.assign(p.N, (const Tri::tri_node_t*)NULL);

    p.heap.reserve(p.N);
    for (int i = 0; i < p.N; ++i) {
        p.heap.push_back(i);
        push_heap(p.heap.begin(), p.heap.end(), node_less(p));
    }
}


void add_edge(params& p, int x, int y, int z, char c) {
    --x;
    --y;

    int idx = x + y * p.N;
    p.graph[idx] = z;
    p.labels[idx] = c;
}




int solve(params& p) {

/*
    while (p.heap.size()) {
        int cur = pop_heap(p.heap


    }
*/

    pop_heap(p.heap.begin(), p.heap.end());
    int v = p.heap[p.heap.size()-1];
    

    return p.dist[v];
}


int main(int argc, const char* argv[]) {

    params p;

    cin >> p.N >> p.M;

    init_graph(p);

    for (int i = 0; i < p.M; ++i) {
        int X, Y, Z;
        char C;

        cin >> X >> Y >> Z >> C;

        add_edge(p, X, Y, Z, C);
    }

    cin >> p.s;

    get_substrings(p);

#if defined DEBUG
    p.subs.print(cout);
#endif

    int result = solve(p);

    cout << result << endl;


    return 0;
}
