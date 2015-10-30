


#include <iostream>
#include <vector>


using namespace std;


struct edge_t {
    int end;
    char c;

    edge_t(int e, char ch) :
        end(e),
        c(ch)
    {}
};


struct path_t {
    int orig;
    int start;
    int finish;
    vector<char> path;

    path_t(int o, int s, int f) :
        orig(o),
        start(s),
        finish(f),
        path()
    {}
};


const int NONE = -1;



struct params {
    int N;
    vector<vector<edge_t> > edges;

    vector<vector<path_t> > pathes;
};


void init_params(params& p) {
    p.edges.assign(p.N, vector<edge_t>());
    p.pathes.assign(p.N, vector<path_t>());

}


void add_edge(params& p, int a, int b, char c) {
    --a;
    --b;
    p.edges[a].push_back(edge_t(b, c));
    p.edges[b].push_back(edge_t(a, c));
}


path_t* get_from_memo(params& p, int orig, int node) {

    for (int n = 0; n < p.pathes[node].size(); ++n) {
        if (p.pathes[node][n].orig == orig) { 
            return &p.pathes[node][n];
        }
    }

    return NULL;
}


int cmp_path(const path_t& p1, const path_t& p2) {
    int size1 = p1.path.size();
    int size2 = p2.path.size();

    int i1 = 0;
    int i2 = 0;

    while (i1 < size1 && i2 < size2) {
        if (p1.path[i1] > p2.path[i2])
            return 1;
        else if (p1.path[i1] < p2.path[i2])
            return -1;
        ++i1;
        ++i2;
    }

    if (i1 < size1)
        return 1;

    else if (i2 < size2)
        return -1;

    return p1.finish - p2.finish;
}


void get_best_path(params& p, path_t& start_point) {
    int n = start_point.start;

    // try memo
    path_t* path_memo = get_from_memo(p, start_point.orig, n);
    if (NULL != path_memo) {
        start_point = *path_memo;
        return;
    }
    

    vector<edge_t*> best_edges;

    for (int e = 0; e < p.edges[n].size(); ++e) {
        edge_t& edge = p.edges[n][e];

        if (start_point.orig == edge.end)
            continue;

        if (!best_edges.size()) {
            best_edges.push_back(&edge);
        }
        else if (best_edges[0]->c == edge.c) {
            best_edges.push_back(&edge);
        }
        else if (best_edges[0]->c < edge.c) {
            best_edges.clear();
            best_edges.push_back(&edge);
        }
    }

    path_t best_path(NONE, NONE, NONE);
    char best_c = NONE;
    
    for (int e = 0; e < best_edges.size(); ++e) { 
        path_t tmp(n, best_edges[e]->end, NONE);
        get_best_path(p, tmp); 

        // remember what I've just found
        p.pathes[best_edges[e]->end].push_back(tmp);

        if (best_path.finish == NONE || 0 > cmp_path(best_path, tmp)) {
            best_path = tmp;
            best_c = best_edges[e]->c;
        }
    } 

    if (best_path.finish == NONE)
        start_point.finish = n;
    else {
        start_point.path.push_back(best_c);
        start_point.path.insert(start_point.path.end(), best_path.path.begin(), best_path.path.end());
        start_point.finish = best_path.finish;
    }

}



void solve(params& p) {

    for (int n = 0; n < p.N; ++n) {
        vector<edge_t*> best_edges;

        for (int e = 0; e < p.edges[n].size(); ++e) {
            edge_t& edge = p.edges[n][e];
            if (!best_edges.size()) {
                best_edges.push_back(&edge);
            }
            else if (best_edges[0]->c == edge.c) {
                best_edges.push_back(&edge);
            }
            else if (best_edges[0]->c < edge.c) {
                best_edges.clear();
                best_edges.push_back(&edge);
            }
        }

        path_t best_path(NONE, NONE, NONE);
    
        for (int e = 0; e < best_edges.size(); ++e) { 
            path_t tmp(n, best_edges[e]->end, NONE);
            get_best_path(p, tmp); 

            // remember what I've just found
            p.pathes[best_edges[e]->end].push_back(tmp);

            if (best_path.finish == NONE || 0 > cmp_path(best_path, tmp)) {
                best_path = tmp;
            }
        } 

        // output the result
        if (best_path.finish == NONE)
            best_path.finish = 0;
        cout << (best_path.finish + 1) << " ";
    }

    cout << endl;
}


int main(int argc, const char* argv[]) {

    params p;

    cin >> p.N;

    init_params(p);

    for (int i = 0; i < p.N-1; ++i) {
        int a, b;
        char c;
        cin >> a >> b >> c;
        add_edge(p, a, b, c);
    }


    solve(p);


    return 0;
}
