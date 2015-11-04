


#include <iostream>
#include <vector>
#include <memory>
#include <sstream>


using namespace std;


const int NONE = -1;




////////////////////////////////////////////////////
// KEY

const int MAX_DIG = 90;
const int TOP_DIG = MAX_DIG + 1;



// actually a number of previous characters, not a digit simbol
inline
bool is_digit(char c) {
    return 0 <= c && c <= MAX_DIG;
}


// key's format: <letter>[<num>]
//    "aa3b12de"
//    "aaaaa" or "a5"
template<typename It>
int cmp_key(It beg1, It end1, It beg2, It end2) {
    size_t s1 = end1 - beg1;
    size_t s2 = end2 - beg2;

    if (!s1 && !s2)
        return 0;
    else if (!s1)
        return -1;
    else if (!s2)
        return 1;


    char c1 = *beg1;
    char c2 = *beg2;

    It tmp1 = beg1; ++tmp1;
    It tmp2 = beg2; ++tmp2;
        
    char n1 = tmp1 != end1 ? (is_digit(*tmp1) ? *(++beg1) : 1) : 1;
    char n2 = tmp2 != end2 ? (is_digit(*tmp2) ? *(++beg2) : 1) : 1;


    while (beg1 != end1 && beg2 != end2) {

        if (c1 > c2) 
            return 1;
        else if (c1 < c2)
            return -1;

        if (n1 < n2) {
            n2 -= n1;
            n1 = 0;
        }
        else {
            n1 -= n2;
            n2 = 0;
        }

        if (!n1) {
            ++beg1;
            if (beg1 != end1) {
                c1 = *beg1;
                tmp1 = beg1; ++tmp1;
                n1 = tmp1 != end1 ? (is_digit(*tmp1) ? *(++beg1) : 1) : 1;
            }
        }
        if (!n2) {
            ++beg2;
            if (beg2 != end2) {
                c2 = *beg2; 
                tmp2 = beg2; ++tmp2;
                n2 = tmp2 != end2 ? (is_digit(*tmp2) ? *(++beg2) : 1) : 1;
            }
        }

    }

    if (!n1 && !n2)
        return 0;
    else if (!n1)
        return -1;
    return 1;
}


class key_t {
public:
    typedef unsigned char CHAR;
    typedef vector<CHAR>::iterator iterator;

private:
    vector<CHAR> storage;

public:
    
    iterator begin() { return storage.begin(); }
    iterator end() { return storage.end(); }

    size_t size() const {return storage.size();}

    void push_back(CHAR c) { 
        size_t s = size();


        if (is_digit(c)) {
            if (is_digit(storage[s-1])) {
                if (TOP_DIG > int(storage[s-1]) + c - 1) {
                    storage[s-1] += c - 1;
                }
                else {
                    storage.push_back(storage[s-2]);
                    char v = int(storage[s-1]) + c - 1 - MAX_DIG;
                    storage[s-1] = MAX_DIG;
                    if (1 < v)
                        storage.push_back(v);
                }
            }
            else {
                storage.push_back(c);
            }
        }
        else if (2 <= s) {
            if (is_digit(storage[s-1])) {
                if (storage[s-2] == c) {
                    if (TOP_DIG > int(storage[s-1]) + 1) {
                        ++storage[s-1];
                    }
                    else {
                        //storage[s-1] = MAX_DIG;
                        storage.push_back(storage[s-2]);
                    }
                }
                else 
                    storage.push_back(c);
            }
            else if (storage[s-1] == c) {
                storage.push_back((char)2);
            }
            else
                storage.push_back(c);
        }
	else if (1 == s && storage[s-1] == c) {
            storage.push_back((char)2);
        }
        else {
            storage.push_back(c);
        } 
    }

    template<typename T>
    void append(T&& other) {
        size_t new_size = storage.size() + other.size();
        if (new_size > storage.capacity())
            storage.reserve(new_size * 2);

        storage.insert(storage.end(), other.begin(), other.end());
    }

    template<typename T>
    void append_push(T&& other) {
        size_t new_size = storage.size() + other.size();
        if (new_size > storage.capacity())
            storage.reserve(new_size * 2);

        if (2 <= other.size()) {
            typename remove_reference<T>::type::iterator it = other.begin();
            push_back(*(it++));
            push_back(*(it++));

            storage.insert(storage.end(), it, other.end());
        }
        else {
            for (typename remove_reference<T>::type::iterator it = other.begin(); it != other.end(); ++it)
                push_back(*it);
        }
    }
        

    friend key_t make_key(const char* s); 


    ostream& print(ostream& os) const {
        for (int i = 0; i < storage.size(); ++i)
            os << (char)(is_digit(storage[i]) ? storage[i] + '0' : storage[i]);
        return os;
    }
};

ostream& operator<< (ostream& os, const key_t& key) {
    return key.print(os);
}


key_t make_key(const char* s) {
    key_t key;
    unsigned char dig = 0;

    while (*s) {
        if ('0' <= *s && *s <= '9')
            dig = dig * 10 + *s - '0';
            //key.storage.push_back(*s - '0');
        else {
            if (dig != 0) {
                key.storage.push_back(dig);
                dig = 0;
            }
            key.storage.push_back(*s);
        }
        ++s;
    }

    if (dig != 0) 
        key.storage.push_back(dig);

    return key;
}




// END OF KEY
////////////////////////////////////////////////////




////////////////////////////////////////////////////
// EDGE


struct edge_t {
    int end;
    char c;
};


// END OF EDGE
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// PATH

struct path_t {
    int dir;
    int finish;
    key_t key;
};

int cmp_path(path_t& p1, path_t& p2) {
    int ret = cmp_key(p1.key.begin(), p1.key.end(), p2.key.begin(), p2.key.end());

    if (0 == ret) {
        return p1.finish - p2.finish;
    }

    return ret;
}

// END OF PATH
////////////////////////////////////////////////////


////////////////////////////////////////////////////
// NODE

struct node_t {

    typedef vector<path_t>::iterator path_iterator;

    vector<path_t> pathes;

    vector<edge_t> edges_hi;
    vector<edge_t> edges_lo;

    int skept_dir;

    node_t() :
        pathes(),
        edges_hi(),
        edges_lo(),
        skept_dir(NONE)
    {}


    void add_edge(edge_t&& e) {
        if (0 == edges_hi.size()) {
            edges_hi.push_back(e);
        }
        else if (edges_hi[0].c == e.c) {
            edges_hi.push_back(e);
        }
        else if (edges_hi[0].c < e.c) {
            edges_lo.assign(edges_hi.begin(), edges_hi.end());
            edges_hi.push_back(e);
        }
        else {
            if (0 == edges_lo.size()) {
                edges_lo.push_back(e);
            }
            else if (edges_lo[0].c == e.c) {
                edges_lo.push_back(e);
            }
            else if (edges_lo[0].c < e.c) {
                edges_lo.clear();
                edges_lo.push_back(e);
            }
            // else - just forget the edge
        }
    }

    void purge_edges() {
        if (1 < edges_hi.size()) {
            edges_lo.clear();
        }
    }
};

// END OF NODE
////////////////////////////////////////////////////





////////////////////////////////////////////////////
// PARAMS

struct params {
    int N;
    vector<node_t> nodes;
};

void init_params(params& p) {
 
    p.nodes.assign(p.N, node_t());

}


void add_edge(params& p, int a, int b, char c) {
    --a;
    --b;
    p.nodes[a].add_edge(edge_t{b, c});
    p.nodes[b].add_edge(edge_t{a, c});
}

// END OF PARAMS
////////////////////////////////////////////////////




void get_best_path(params& p, int prev_idx, int node_idx, path_t& path) {

}




int get_destination(params& p, int prev_idx, int node_idx, path_t* ret_path=NULL) {
    node_t& node = p.nodes[node_idx];


    // check if it's leaf
    if (1 == node.edges_hi.size() && node.edges_hi[0].end == prev_idx) {
        return node_idx;
    }


    // node has children
    for (int e = 0; e < node.edges_hi.size(); ++e) {
        if (prev_idx == node.edges_hi[e].end) {
            node.skept_dir = prev_idx;
            continue;
        }

        path_t next_path;
        get_best_path(p, node_idx, node.edges_hi[e].end, next_path);


        path_t cur_path;
        cur_path.dir = node.edges_hi[e].end;
        cur_path.finish = next_path.finish;
        cur_path.key.push_back(node.edges_hi[e].c);
        cur_path.key.append_push(next_path.key);


        if (0 == node.pathes.size()) {
            node.pathes.push_back(move(cur_path));
        }
        else {
            if (0 < cmp_path(cur_path, node.pathes[0])) {
                node.pathes.insert(node.pathes.begin(), move(cur_path));
            }
            else {
                node.pathes.push_back(move(cur_path));
            }
        }
    }

    if (NONE != node.skept_dir) {
        for (int e = 0; e < node.edges_lo.size(); ++e) {
            if (prev_idx == node.edges_lo[e].end) {
                node.skept_dir = prev_idx;
                continue;
            }

        }
    }
    else {
        node_t::path_iterator second = node.pathes.begin(); ++second;
        node.pathes.erase(second, node.pathes.end());
    }

    return node.pathes[0].finish;
}


void solve(params& p) {

    string buffer;
    buffer.reserve(p.N * 5);
    stringstream ss(buffer);


    for (int i = 0; i < p.N; ++i) {
        ss << get_destination(p, NONE, i) << " ";
    }


    cout << ss.str() << endl;
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

    // release some memory if possible
    for (int i = 0; i < p.N; ++i) {
        p.nodes[i].purge_edges();
    }


    solve(p);


    return 0;
}
