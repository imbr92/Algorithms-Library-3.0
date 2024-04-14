/*  Overview: 
        - Binary Lifting LCA  
    Parts
        - n = number of nodes in the tree
        - lg = smallest power of 2 >= n
        - t = time in dfs traversal
        - tin[v] = time when the subtree rooted at v (w.r.t the tree rooted at root) was entered.
        - tout[v] = time when the subtree rooted at v (w.r.t the tree rooted at root) was exited.
        - adj[][] = adjacency list of original tree
        - up[v][i] = Ancestor gotten from moving up from v 2^i times. Does not move past the root.
    Methods:
        - dfs(v, p):        Computes up[][], tin[] and tout[] in O(n log n).
        - calculate(v):     Wrapper for precomputation of LCAs for tree rooted at v. O(n log n)
        - chk(u, v):        Returns true iff u is an ancestor of v (or v itself). O(1)
        - lca(u, v):        Returns the least common ancestor between nodes u and v in the tree rooted at root. O(log n)
    Verification: 
        - https://judge.yosupo.jp/submission/67943
    Assumptions:
        - n < (1e9 + 7)/2 (possibly still doesnt break until n >= MAX_INT/2 though)
        - Overshooting root should return root
        - Compiler has __builtin_clz
*/

struct LCA{
    const int INF = 1e9 + 7;
    int n, lg, t;
    vector<int> tin, tout;
    vector<vector<int>> up, adj;

    LCA(int n, const vector<vector<int>> &adj): n(n), adj(adj){
        lg = 0, t = 0;
        while((1LL << lg) < n) ++lg;
        assert((1LL << lg) >= n && (1LL << (lg - 1)) < n);
    }

    void dfs(int v, int p){
        tin[v] = ++t, up[v][0] = p;
        for(int i = 1; i <= lg; ++i){
            up[v][i] = up[up[v][i - 1]][i - 1];
        }
        for(auto i : adj[v]){
            if(i == p) continue;
            dfs(i, v);
        }
        tout[v] = ++t;
    }

    inline void calculate(int root){
        tin.assign(n, INF); tout.assign(n, -INF);
        up.assign(n, vector<int>(lg + 1, root));
        dfs(root, root);
    }

    inline bool chk(int u, int v){ // returns true if u is an ancestor of v (including v itself)
        return tin[u] <= tin[v] && tout[u] >= tout[v];
    }

    inline int lca(int u, int v){
        if(chk(u, v)) return u;
        if(chk(v, u)) return v;
        for(int i = lg; i >= 0; --i){ // lift u upwards
            if(!chk(up[u][i], v)){
                u = up[u][i];
            }
        }
        return up[u][0];
    }
};