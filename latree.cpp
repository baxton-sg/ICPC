


#include <iostream>
#include <vector>
#include <memory>
#include <sstream>


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
    int dir;
    int finish;
    vector<char> path;

    path_t(int o, int s, int f, int d) :
        orig(o),
        start(s),
        dir(d),
        finish(f),
        path()
    {
        path.reserve(20000);
    }
};


const int NONE = -1;



struct params {
    int N;
    vector<vector<edge_t> > edges;

    vector<vector<path_t> > pathes;
};


void init_params(params& p) {
    vector<edge_t> tmp_edge;
    vector<path_t> tmp_path;

    tmp_edge.reserve(1000);
    tmp_path.reserve(20000);

 
    p.edges.assign(p.N, tmp_edge);
    p.pathes.assign(p.N, tmp_path);

}


void add_edge(params& p, int a, int b, char c) {
    --a;
    --b;
    p.edges[a].push_back(edge_t(b, c));
    p.edges[b].push_back(edge_t(a, c));
}


path_t* get_from_memo(params& p, int orig, int node) {

    // 1 or 2 pathes always!!!
    if (0 < p.pathes[node].size()) {
        if (p.pathes[node][0].dir != orig)
            return &p.pathes[node][0];
        else if (1 < p.pathes[node].size())
            return &p.pathes[node][1];
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



    if (NULL != path_memo && (start_point.orig == path_memo->orig || 2 == p.pathes[n].size())) {
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

/*
    if (NULL != path_memo) {
        if (0 < best_edges.size()) {
            if (best_edges[0]->end == path_memo->dir) { 
                start_point = *path_memo;
                return;
            }
        }
	else { 
//            start_point = *path_memo;
//            return;
        }
    }
*/


    path_t best_path(NONE, NONE, NONE, NONE);
    char best_c = NONE;

    
    for (int e = 0; e < best_edges.size(); ++e) { 
        if (path_memo && path_memo->dir == best_edges[e]->end)
            continue;

        path_t tmp(n, best_edges[e]->end, NONE, NONE);

        get_best_path(p, tmp); 

        if (best_path.finish == NONE || 0 > cmp_path(best_path, tmp)) {
            best_path = tmp;
            best_c = best_edges[e]->c;
        }

        tmp.path.clear();
    } 

    if (best_path.finish == NONE)
        if (path_memo)
            start_point = *path_memo;
        else
            start_point.finish = n;
    else {
        if (0 == p.pathes[best_path.start].size())
            p.pathes[best_path.start].push_back(best_path);
        else if (1 == p.pathes[best_path.start].size() && best_path.orig != p.pathes[best_path.start][0].orig) {
            if (0 > cmp_path(p.pathes[best_path.start][0], best_path)) {
                p.pathes[best_path.start].push_back(p.pathes[best_path.start][0]);
                p.pathes[best_path.start][0] = best_path;
            }
            else {
                p.pathes[best_path.start].push_back(best_path);
            }
        }

        start_point.path.push_back(best_c);
        start_point.path.insert(start_point.path.end(), best_path.path.begin(), best_path.path.end());
        start_point.finish = best_path.finish;
        start_point.dir = best_path.start;

        if (path_memo) {
            if (0 > cmp_path(*path_memo, start_point)) {
                p.pathes[n].push_back(*path_memo);
                p.pathes[n][0] = start_point;
            }
            else {
                p.pathes[n].push_back(start_point);
                start_point = *path_memo;
            }
        }
    }

}



void solve(params& p) {

    string buffer;
    buffer.reserve(p.N * 5);
    stringstream ss(buffer);

    auto_ptr<path_t> pp(new path_t(NONE, 0, NONE, NONE));

    for (int n = 0; n < p.N; ++n) {

    
        path_t& tmp = *pp.get();
        tmp.orig = NONE;
        tmp.start = n;
        tmp.finish = NONE;
        tmp.dir = NONE;
        get_best_path(p, tmp); 


        // output the result
        if (tmp.finish == NONE)
            tmp.finish = 0;
        ss << (tmp.finish + 1) << " ";

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


    solve(p);


    return 0;
}
