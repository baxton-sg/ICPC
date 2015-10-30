


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

    path_t(int o, int s, int f) :
        orig(o),
        start(s),
        finish(f)
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


int get_from_memo(params& p, int orig, int node) {
    int finish = NONE;

    for (int n = 0; n < p.pathes[node].size(); ++n) {
        if (p.pathes[node][n].orig == orig) { 
            finish = p.pathes[node][n].finish;
            break;
        }
    }

    return finish;
}


int cmp_path(const params& p, int cur1, int finish1, int cur2, int finish2) {

    if (cur1 == finish1 && cur2 == finish2)
        return 0;
    else if (cur1 == finish1)
        return -1;
    else if (cur2 == finish2)
        return 1;

    const edge_t* edge1 = NULL;
    const edge_t* edge2 = NULL;

    for (int e = 0; e < p.edges[cur1].size(); ++e) {
        if (edge1 == NULL || edge1->c < p.edges[cur1][e].c)
            edge1 = &p.edges[cur1][e];
    }

    for (int e = 0; e < p.edges[cur2].size(); ++e) {
        if (edge2 == NULL || edge2->c < p.edges[cur2][e].c)
            edge2 = &p.edges[cur2][e];
    }

//    if (edge1 && edge2) {
        if (edge1->c == edge2->c)
            return cmp_path(p, edge1->end, finish1, edge2->end, finish2);
        return edge1->c - edge2->c;
//    }   

    return finish1 - finish2;
}


void get_best_path(params& p, path_t& start_point) {
    int n = start_point.start;

    // try memo
    int finish = get_from_memo(p, start_point.orig, n);
    if (NONE != finish) {
        start_point.finish = finish;
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
    
    for (int e = 0; e < best_edges.size(); ++e) { 
        path_t tmp(n, best_edges[e]->end, NONE);
        get_best_path(p, tmp); 

        // remember what I've just found
        p.pathes[best_edges[e]->end].push_back(tmp);

        if (best_path.finish == NONE || 0 > cmp_path(p, best_path.start, best_path.finish, tmp.start, tmp.finish)) {
            best_path = tmp;
        }
    } 

    if (best_path.finish == NONE)
        start_point.finish = n;
    else
        start_point.finish = best_path.finish;

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

            if (best_path.finish == NONE || 0 > cmp_path(p, best_path.start, best_path.finish, tmp.start, tmp.finish)) {
                best_path = tmp;
            }
        } 

        // output the result
        cout << (best_path.finish + 1);
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
