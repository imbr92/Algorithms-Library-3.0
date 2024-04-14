/*  Overview: 
        - Minimum Diameter Spanning Tree (Spanning tree rooted at absolute center of the tree (node n + 1, could be in the middle of an edge)).
        - If true absolute center is a pre-existing node, the n + 1th node will be arbitrarily close to that node.
        - Requires positive edge weights and a simple graph
        - Works with integral weights (w_i < MAX_INT for all i) if weight is integral. 
    Parts:
        - res = Absolute center of MDST. {Offset from u, Diameter of MDST, edge{u, v, w}}
        - ord[i] = Order of nodes from least to greatest distances from node i
        - d[i][j] = Minimal distance from node i to node j
        - adj[i][j] = Adjacency matrix of original graph
        - edges[] = List of all edges in the original graph
        - tree[] = Edges remaining in the final MDST
        - r[i] = Distance from absolute center to node i
        - INF = Infinity (> max length)
        - eps = Arbitrary precision
    Methods:
        - preprocess(): Finds All Pairs Shortest Paths with Floyd-Warshall and preprocesses sorted orderings (non-increasing a_i's). O(V^3)
        - calcSPT({{max dist from absolute center, absolute center}, Edge of absolute 
          center}):     Finds the shortest paths tree from the given absolute center. O(V^2)
        - build():      Runs entire building process. Finds APSP, then keeps track of relevant constraints 
                        for the hull while checking intersections and updating the global global minimum. 
                        After this it calls calcSPT() to create the MDST originating from the absolute center. O(V^3) 
    Sources: 
        - Play With Trees Online Contest Problem C
            - Problem: https://adn.botao.hu/adn-backup/blog/attachments/month_0705/n2007531153220.pdf 
            - Editorial: https://adn.botao.hu/adn-backup/blog/attachments/month_0705/32007531153238.pdf (Pages 6 - 9)
        - https://github.com/spaghetti-source/algorithm/blob/master/graph/kariv_hakimi.cc
    Verification: 
        - https://codeforces.com/contest/266/submission/87250689
        - SPOJ PT07C     */

template<typename T> struct edge{
    int u, v; T w;
    edge() {}
    edge(int u, int v, T w): u(u), v(v), w(w) {}
};

template<typename T> struct MDST{
    T INF; double eps = 1e-15;
    int n;
    vector<bool> vis;
    vector<T> r; // radius from absolute center
    vector<pair<int, int>> tree; // MDST stored as pairs {u, v}
    vector<edge<T>> edges; // Edge list
    vector<vector<int>> ord;
    vector<vector<T>> adj, d; // Adjacency Matrix, APSP Matrix
    struct ans{
        edge<T> cen; // Central edge
        T offset; // offset from u 
        T max_dist; // Diameter of the MDST
    };
    ans res;
    MDST(int sz, vector<edge<T>> &e, const T INF): n(sz), INF(INF) {
        adj.assign(n, vector<T>(n, INF));
        ord.assign(n, vector<int>(n, 0));
        for(int i = 0; i < n; ++i) iota(ord[i].begin(), ord[i].end(), 0), adj[i][i] = 0;
        for(auto i : e){
            i.w *= 2, edges.pb(i); // Allows us to work purely with integers if weights are integral
            ckmin(adj[i.u][i.v], i.w);
            ckmin(adj[i.v][i.u], i.w);
        }
    }
    void calcSPT(int v){ // Recursively constructs Shortest Paths Tree
        vis[v] = true;
        for(int i = 0; i < n; ++i){
            if(abs(r[v] + adj[v][i] - r[i]) < eps){
                if(!vis[i]){
                    tree.pb({v, i});
                    calcSPT(i);
                }
            }
        }
    }
    void preprocess(){ // Floyd-Warshall + Sorting Preprocessing
        d = adj;
        for(int k = 0; k < n; ++k)
            for(int i = 0; i < n; ++i)
                for(int j = 0; j < n; ++j)
                    if(d[i][k] < INF && d[k][j] < INF)
                        ckmin(d[i][j], d[i][k] + d[k][j]);
        for(int i = 0; i < n; ++i){ // sort by decreasing a_i's
            sort(ord[i].begin(), ord[i].end(), [&](const int &a, const int &b){
                return d[i][a] > d[i][b];
            });
        }
    }
    void build(){
        preprocess();
        res.cen = edges[0], res.max_dist = INF;
        for(auto e : edges){
            int &u = e.u, &v = e.v; T &w = e.w;
            if(d[u][ord[v][0]] + d[v][ord[u][0]] > 2 * res.max_dist) continue; // Halpern bound
            if(ord[u][0] == ord[v][0]) continue;
            vector<int> &co = ord[u];
            int k = 0; // Last constraint on hull
            T x = 0, y = min(d[u][co[0]], d[v][co[0]] + w), xi, yi;
            for(int i = 1; i < n; ++i){
                if(d[v][co[k]] < d[v][co[i]]){ // b_i greater for current value (guaranteed to have lower a_i < a_k because of ordering)
                    xi = (d[v][co[k]] - d[u][co[i]] + w)/2;
                    yi = xi + d[u][co[i]];
                    if(yi < y) y = yi, x = xi; // If new global min, update
                    k = i;
                }
            }
            yi = min(d[u][co[k]] + w, d[v][co[k]]);
            if(yi < y) y = yi, x = 1; 
            if(y < res.max_dist) res.max_dist = y, res.offset = x, res.cen = e; // Set new absolute center
        }
        // now res is fully set
        r.assign(n, 0); vis.assign(n, false);
        for(int i = 0; i < n; ++i){ // set minimum distances to each node from the absolute center by checking shortest distance to either side of central edge
            r[i] = min(d[res.cen.u][i] + res.offset, d[res.cen.v][i] + res.cen.w - res.offset); 
        }
        if(abs(res.offset) > eps) calcSPT(res.cen.v); // Calculates SPT if non zero offset
        if(res.cen.w > res.offset){
            calcSPT(res.cen.u); // Calculates SPT if zero offset
            if (res.offset > 0) tree.pb({res.cen.u, res.cen.v});
        }
    }
};