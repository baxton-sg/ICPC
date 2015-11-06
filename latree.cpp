


#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include <sstream>
#include <stack>


using namespace std;


const int NONE = -1;




////////////////////////////////////////////////////
// KEY

namespace hr_key {


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
    hr_key::key_t key;
};

int cmp_path(path_t& p1, path_t& p2) {
    int ret = hr_key::cmp_key(p1.key.begin(), p1.key.end(), p2.key.begin(), p2.key.end());

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

    typedef list<edge_t> edge_container;
    typedef edge_container::iterator edge_iterator;   

    typedef vector<path_t>::iterator path_iterator;

    vector<path_t> pathes;

    edge_container edges_hi;
    edge_container edges_lo;


    node_t() :
        pathes(),
        edges_hi(),
        edges_lo()
    {}


    void add_edge(edge_t&& e) {
        if (0 == edges_hi.size()) {
            edges_hi.push_back(e);
        }
        else if (edges_hi.front().c == e.c) {
            edges_hi.push_back(e);
        }
        else if (edges_hi.front().c < e.c) {
            edges_lo.assign(edges_hi.begin(), edges_hi.end());
            edges_hi.clear();
            edges_hi.push_back(e);
        }
        else {
            if (0 == edges_lo.size()) {
                edges_lo.push_back(e);
            }
            else if (edges_lo.front().c == e.c) {
                edges_lo.push_back(e);
            }
            else if (edges_lo.front().c < e.c) {
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




struct call_args {
    int prev_idx;
    int node_idx;
    path_t ret_path;

    call_args(int prev, int cur) :
        p(prms),
        prev_idx(prev),
        node_idx(cur)
    {}
};



int get_destination(params& p, int start_node_idx) {

    stack<call_args*> call_stack;

    call_args* pargs = new call_args(NONE, start_node_idx);
    call_stack.push(pargs);


    while (!call_stack.empty()) {

        unique_ptr<call_args> args(call_stack.top());
        call_stack.pop();

        int prev_idx = args->prev_idx;
        int node_idx = args->node_idx;
        path_t* ret_path = &args->ret_path;


        node_t& node = p.nodes[node_idx];

        size_t pathes_num = node.pathes.size();

        if (0 == pathes_num) {
       
            // check if it's leaf
            if (1 == node.edges_hi.size() && 
                0 == node.edges_lo.size() && 
                node.edges_hi.front().end == prev_idx) {
    
                if (ret_path) {
                    ret_path->finish = node_idx;
                }

                return node_idx;
            }


            // node has children
            for (node_t::edge_iterator e = node.edges_hi.begin(); e != node.edges_hi.end(); ) {
                if (prev_idx == e->end) {
                    ++e;
                    continue;
                }

                path_t next_path;
                args.prev_idx = node_idx;
                args.node_idx = e->end;
                args.ret_path = &next_path;
                get_destination(args);


                path_t cur_path;
                cur_path.dir = e->end;
                cur_path.finish = next_path.finish;
                cur_path.key.push_back(e->c);
                cur_path.key.append_push(next_path.key);


                if (0 == pathes_num) {
                    node.pathes.push_back(move(cur_path));
                    ++pathes_num;
                }
                else {
                    if (0 < cmp_path(cur_path, node.pathes[0])) {
                        node.pathes.insert(node.pathes.begin(), move(cur_path));
                        ++pathes_num;

                        if (2 < pathes_num) {
                            node.pathes.pop_back();
                            --pathes_num;
                        }
                    }
                    else {
                        if (1 == pathes_num) {
                            node.pathes.push_back(move(cur_path));
                            ++pathes_num;
                        }
                        else if (2 == pathes_num && 0 < cmp_path(cur_path, node.pathes[1])) {
                            node.pathes[1] = move(cur_path);
                        }
                    }
                }

                // finally I can delete this edge as processed
                node.edges_hi.erase(e++);
            }

            if (1 == node.edges_hi.size()) {
                for (node_t::edge_iterator e = node.edges_lo.begin(); e != node.edges_lo.end(); ) {

                    path_t next_path;
                    args.prev_idx = node_idx;
                    args.node_idx = e->end;
                    args.ret_path = &next_path;
                    get_destination(args);


                    path_t cur_path;
                    cur_path.dir = e->end;
                    cur_path.finish = next_path.finish;
                    cur_path.key.push_back(e->c);
                    cur_path.key.append_push(next_path.key);


                    if (0 == pathes_num) {
                        node.pathes.push_back(move(cur_path));
                        ++pathes_num;
                    }
                    else {
                        if (0 < cmp_path(cur_path, node.pathes[0])) {
                            node.pathes.insert(node.pathes.begin(), move(cur_path));
                            ++pathes_num;

                            if (2 < pathes_num) {
                                node.pathes.pop_back();
                                --pathes_num;
                            }
                        }
                        else {
                            if (1 == pathes_num) {
                                node.pathes.push_back(move(cur_path));
                                ++pathes_num;
                            }
                            else if (2 == pathes_num && 0 < cmp_path(cur_path, node.pathes[1])) {
                                node.pathes[1] = move(cur_path);
                            }
                        }
                    }

                    node.edges_lo.erase(e++);
                }
            }

        }
        else if (1 == pathes_num) {
            // leaf
            if (0 == node.edges_hi.size() && 0 == node.edges_lo.size()) {
                if (node.pathes[0].dir == prev_idx) {
                    if (ret_path) {
                        ret_path->dir = NONE;
                        ret_path->finish = node_idx;
                    }
    
                    return node_idx;
                }
            }
            else if (1 == node.edges_hi.size()) {
                node_t::edge_iterator e = node.edges_hi.begin();

                if (prev_idx != e->end) {

                    path_t next_path;
                    args.prev_idx = node_idx;
                    args.node_idx = e->end;
                    args.ret_path = &next_path;
                    get_destination(args);

                    path_t cur_path;
                    cur_path.dir = e->end;
                    cur_path.finish = next_path.finish;
                    cur_path.key.push_back(e->c);
                    cur_path.key.append_push(next_path.key);

                    if (0 < cmp_path(cur_path, node.pathes[0])) {
                        node.pathes.insert(node.pathes.begin(), move(cur_path));
                    }
                    else {
                        node.pathes.push_back(move(cur_path));
                    }

                    ++pathes_num;
    
                    node.edges_hi.erase(e);
                }
            }
            else if (0 < node.edges_lo.size()) {
                for (node_t::edge_iterator e = node.edges_lo.begin(); e != node.edges_lo.end(); ) {
                    if (prev_idx == e->end) {
                        ++e;
                        continue;
                    }

                    path_t next_path;
                    args.prev_idx = node_idx;
                    args.node_idx = e->end;
                    args.ret_path = &next_path;
                    get_destination(args);


                    path_t cur_path;
                    cur_path.dir = e->end;
                    cur_path.finish = next_path.finish;
                    cur_path.key.push_back(e->c);
                    cur_path.key.append_push(next_path.key);


                    if (0 == pathes_num) {
                        node.pathes.push_back(move(cur_path));
                        ++pathes_num;
                    }
                    else {
                        if (0 < cmp_path(cur_path, node.pathes[0])) {
                            node.pathes.insert(node.pathes.begin(), move(cur_path));
                            ++pathes_num;

                            if (2 < pathes_num) {
                                node.pathes.pop_back();
                                --pathes_num;
                            }
                        }
                        else {
                            if (1 == pathes_num) {
                                node.pathes.push_back(move(cur_path));
                                ++pathes_num;
                            }
                            else if (2 == pathes_num && 0 < cmp_path(cur_path, node.pathes[1])) {
                                node.pathes[1] = move(cur_path);
                            }
                        }
                    }

                    node.edges_lo.erase(e++);
                }
            }
        }
        else { // 2 pathes
            if (1 == node.edges_hi.size()) {
                node_t::edge_iterator e = node.edges_hi.begin();

                if (prev_idx != e->end) {

                    path_t next_path;
                    args.prev_idx = node_idx;
                    args.node_idx = e->end;
                    args.ret_path = &next_path;
                    get_destination(args);

                    path_t cur_path;
                    cur_path.dir = e->end;
                    cur_path.finish = next_path.finish;
                    cur_path.key.push_back(e->c);
                    cur_path.key.append_push(next_path.key);

                    if (0 < cmp_path(cur_path, node.pathes[0])) {
                        node.pathes.insert(node.pathes.begin(), move(cur_path));
                        ++pathes_num;
    
                        if (2 < pathes_num) {
                            node.pathes.pop_back();
                            --pathes_num;
                        }
                    }
                    else {
                        if (0 < cmp_path(cur_path, node.pathes[1])) {
                            node.pathes[1] = move(cur_path);
                        }
                    }

                    ++pathes_num;

                    node.edges_hi.erase(e);
                }
            }
            else if (1 == node.edges_lo.size()) {
                node_t::edge_iterator e = node.edges_lo.begin();

                if (prev_idx != e->end) {

                    path_t next_path;
                    args.prev_idx = node_idx;
                    args.node_idx = e->end;
                    args.ret_path = &next_path;
                    get_destination(args);

                    path_t cur_path;
                    cur_path.dir = e->end;
                    cur_path.finish = next_path.finish;
                    cur_path.key.push_back(e->c);
                    cur_path.key.append_push(next_path.key);

                    if (0 < cmp_path(cur_path, node.pathes[0])) {
                        node.pathes.insert(node.pathes.begin(), move(cur_path));
                        ++pathes_num;
    
                        if (2 < pathes_num) {
                            node.pathes.pop_back();
                            --pathes_num;
                        }
                    }
                    else {
                        if (0 < cmp_path(cur_path, node.pathes[1])) {
                            node.pathes[1] = move(cur_path);
                        }
                    }

                    ++pathes_num;

                    node.edges_hi.erase(e);
                }
            }
        }


    }    // while !call_stack.empty



    if (2 == pathes_num) {
        if (prev_idx == node.pathes[0].dir) {
            if (ret_path) { 
                *ret_path = node.pathes[1];
            }
            return node.pathes[1].finish;
        }
    }

    if (ret_path)
        *ret_path = node.pathes[0]; 

    return node.pathes[0].finish;
}


void solve(params& p) {

    string buffer;
    buffer.reserve(p.N * 5);
    stringstream ss(buffer);


//    int i = 1;
//    while (i)
//        ;

    call_args args(p, NONE, NONE, NULL);

    for (int i = 0; i < p.N; ++i) {
        args.node_idx = i;
        ss << (1 + get_destination(args)) << " ";
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

