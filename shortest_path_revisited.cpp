


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





template<typename T, typename LESS>
class heap_t {
public:
    typedef pair<int, T> item_t;
    typedef vector<item_t> storage_t;

private:
    storage_t storage;
    vector<int> indices;
    LESS cmp_less;

public:
    heap_t(LESS less, size_t reserve=0) :
        storage(),
        indices(),
        cmp_less(less)
    {
        if (reserve) {
            storage.reserve(reserve);
            indices.reserve(reserve);
        }
    }

    int push_heap(int val) {
        int id = indices.size();
        int i = storage.size();

        storage.push_back(make_pair(id, val));
        indices.push_back(i);

        heapify_up(i);

        return id;
    } 

    T& top() {
        return storage[0].second;
    }

    void pop_heap() {
        storage[0] = storage[storage.size()-1];
        storage.pop_back();

        indices[storage[0].first] = 0; 

        heapify_down(0);
    }

    void change_key(int id, T val) {
        int i = indices[id];
        if (cmp_less(storage[i].second, val)) { 
            storage[i].second = val;
            heapify_down(i);
        }
        else {
            storage[i].second = val;
            heapify_up(i);
        }
    }

private:
    void heapify_up(int i) {
        if (0 == i)
            return;

        int parity = i & 1;
        int parent = (i - (2 - parity)) / 2;

        if (cmp_less(storage[i].second, storage[parent].second)) {
            // swap
            item_t tmp = storage[parent];
            storage[parent] = storage[i];
            storage[i] = tmp;

            indices[storage[parent].first] = parent;
            indices[storage[i].first] = i;

            heapify_up(parent);
        }
    }

    void heapify_down(int i) {
//        if (i == storage.size() - 1)
//            return;

        int ch1 = i * 2 + 1;
        int ch2 = i * 2 + 2;
        int ch = ch2;

        if (storage.size() > ch1 && storage.size() > ch1) {
            if (cmp_less(storage[ch1].second, storage[ch2].second)) {
                ch = ch1;
            }
            else {
                ch = ch2;
            }
        }
        else if (storage.size() > ch1) {
            ch = ch1;
        }
        else if (storage.size() > ch2) {
            ch = ch2;
        }
        else
            return;


        if (cmp_less(storage[i].second, storage[ch].second)) {
            // swap
            item_t tmp = storage[ch];
            storage[ch] = storage[i];
            storage[i] =  tmp;

            indices[storage[ch].first] = ch;
            indices[storage[i].first] = i;

            heapify_down(ch);
        }
    }

};

template <typename T>
struct node_less {
    const T& p_;
    node_less(const T& p) :
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

        return res;
    }
};

template <typename T>
struct node_greater {
    const T& p_;
    node_greater(const T& p) :
        p_(p)
    {}

    bool operator() (int i1, int i2) {
        bool res = false;

        if (p_.dist[i1] == INF && p_.dist[i2] == INF)
            res = false;
        else if (p_.dist[i1] == INF)
            res = true;
        else if (p_.dist[i2] == INF)
            res = false;
        else
            res = p_.dist[i1] > p_.dist[i2];

        return res;
    }
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

    heap_t<int, node_less<params> > heap;
    //heap_t<int, node_greater<params> > heap;

    
    params() :
        s(""),
        subs(),
        N(0),
        M(0),
        graph(),
        dist(),
        labels(),
        path_data(),
        heap(node_less<params>(*this), 1024)
        //heap(node_greater<params>(*this), 1024)
    {}
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

    for (int i = 0; i < p.N; ++i) {
        p.heap.push_heap(p.dist[i]);
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

    int v = p.heap.top();
    p.heap.pop_heap();

    p.dist[1] = 15;
    p.heap.change_key(1, 15);
    v = p.heap.top();

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