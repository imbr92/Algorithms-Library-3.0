/*  Overview: 
        - 2SAT solver using Kosaraju
    Parts
        - n = number of distinct boolean variables 
        - wks = boolean variable for whether the given 2SAT expression is satisfiable
        - assignment[i] = Value of ith boolean variable in a valid assignment
        - adj[][] = adjacency list of 2SAT expression 
    Methods:
        - add(u, v):        Adds (u or v) clause to 2SAT expresion. O(1)
        - solve():          Checks if there is an assignment to make the expression 
                            satisfiable, and computes it if there is one. O(n + m)
    Verification: 
        - https://judge.yosupo.jp/submission/70020
    Assumptions:
        - For inputs to add, u << 1 given if u is not negated, u << 1 | 1 if u is negated
        - No guarantees on contents of assignment[] if wks is false
*/

// add allows you to add (u or v) clauses
struct TwoSAT{
    struct SCC{
        int n;
        vector<bool> vis;
        vector<int> ord, roots, component, root_nodes;
        vector<vector<int>> adj, radj, adj_scc;
        SCC(int n, vector<vector<int>> &g): n(n){
            adj.assign(n, vector<int>());
            radj.assign(n, vector<int>());
            for(int i = 0; i < n; ++i){
                for(auto j : g[i]){
                    adj[i].push_back(j);
                    radj[j].push_back(i);
                }
            }
        }
        void dfs1(int v){
            vis[v] = true;
            for(auto i : adj[v]){
                if(!vis[i]) dfs1(i);
            }
            ord.push_back(v); // ord in increasing tout order
        }
        void dfs2(int v){
            vis[v] = true;
            component.push_back(v);
            for(auto i : radj[v]){
                if(!vis[i]) dfs2(i);
            }
        }
        //set unique to true if you want scc edges to be unique (slow since unordered_set)
        void calculate(bool unique = false){
            vis.assign(n, false);
            roots.assign(n, -1);
            adj_scc.assign(n, vector<int>());
            for(int i = 0; i < n; ++i){
                if(!vis[i]) dfs1(i);
            }
            reverse(ord.begin(), ord.end());
            vis.assign(n, false);
            for(auto v : ord){
                if(!vis[v]){
                    dfs2(v);
                    int root = component.front();
                    for (auto u : component) roots[u] = root;
                    root_nodes.push_back(root);
                    component.clear();
                }
            }
            if(!unique){
                for(int v = 0; v < n; v++){
                    for(auto u : adj[v]){
                        int root_v = roots[v], root_u = roots[u];
                        if (root_u != root_v){
                            adj_scc[root_v].push_back(root_u);
                        }
                    }
                }
            }
            else{
                struct chash{
                    size_t operator()(const pair<int, int>& p) const{
                        auto hash1 = hash<int>{}(p.first);
                        auto hash2 = hash<int>{}(p.second);
                        return hash1 ^ hash2;
                    }
                };
                unordered_set<pair<int, int>, chash> used;
                for(int v = 0; v < n; v++){
                    for(auto u : adj[v]){
                        int root_v = roots[v], root_u = roots[u];
                        auto key = make_pair(root_v, root_u);
                        if (root_u != root_v && used.find(key) == used.end()){
                            adj_scc[root_v].push_back(root_u);
                            used.insert(key);
                        }
                    }
                }            
            }
        }
    };
    bool wks;
    int n;
    vector<bool> assignment;
    vector<vector<int>> adj;
    TwoSAT(int n): n(n), adj((n << 1), vector<int>()) {}
    inline void add(int u, int v){ // u << 1 for on, u << 1 | 1 for off
        adj[u ^ 1].push_back(v);
        adj[v ^ 1].push_back(u);
    }
    bool solve(){
        wks = true;
        assignment.assign(n, false);
        SCC cond(n << 1, adj); cond.calculate(false);
        vector<int> topo(n << 1, -1); 
        for(int i = 0; i < sz(cond.root_nodes); ++i){
            topo[cond.root_nodes[i]] = i;
        }
        for(int i = 0; i < (n << 1); i += 2){
            if(cond.roots[i] == cond.roots[i ^ 1]){
                wks = false;
                break;
            }
            assignment[i >> 1] = topo[cond.roots[i]] > topo[cond.roots[i ^ 1]];
        }
        return wks;
    }
};