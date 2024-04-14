/*  Overview: 
        - Kosaraju SCC
    Parts
        - n = number of nodes in the directed graph
        - vis[] = visited array, cleared before each set of dfses
        - ord[i] = nodes sorted in decreasing tout order
        - roots[i] = parent of i in condensation graph
        - root_nodes[] = list of nodes remaining in condensation graph in topological order
        - adj_scc[][] = adjacency list of condensation graph
        - adj[][] = adjacency list of input graph
        - radj[][] = reversed adjacency list of input graph
    Methods:
        - dfs1(v):            Computes tout[] in O(n)
        - dfs2(v):            Called on v from decreasing tout order to traverse radj. Gives SCCs one at a time. O(n)
        - calculate(unique):  Computes adj_scc, root_nodes, roots, ord in O(n). If unique = true, adj_scc 
                                has no repeated edges (slow since hashset). 
    Verification: 
        - https://judge.yosupo.jp/submission/68341
*/

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