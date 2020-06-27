/*  Overview: 
        - Standard centroid decomposition
    Parts
        - g[][] = adjacency list of original tree
        - adj[][] = adjacency list of tree after centroid decomposition
        - dead[i] = true if i was already used as a centroid during the decomposition (marked as false again afterwards)
        - SZ = # of nodes in the whole tree
        - _n = # of nodes in the current componenet
        - rt = root of centroid tree
    Methods:
        - get_size(v, p):   Sets size array for all nodes in the current component. O(CC Size)
        - get_cen(v, p):    Uses size information to find centroid of current component. O(CC Size)
        - find(v):          Driver for get_size and get_cen to break a single component into multiple
                            componenets with centroid removed. O(CC Size)
        - dfs(v, p):        DFS using find() to recursively find centroids and build CD tree. O(SZ log_2(SZ))
        - decomp():         Driver for entire decomposition. O(SZ log_2(SZ))
    Verification: 
        - https://codeforces.com/contest/716/submission/85067920
        - https://codeforces.com/contest/342/submission/84929339    */
struct CD{
    int SZ, _n, rt = -1;
    vector<bool> dead;
    vector<int> size;
    vector<vector<int>> adj, g;
    CD(int SZ, vector<vector<int>> &g): SZ(SZ), g(g){
        size.assign(SZ, 0);
        dead.assign(SZ, 0);
        adj.assign(SZ, vector<int>());

    }
    void get_size(int v, int p){
        size[v] = 1;
        for(auto i : g[v]){
            if(dead[i] || i == p) continue;
            get_size(i, v);
            size[v] += size[i];
        }
    }
    int get_cen(int v, int p){
        for(auto i : g[v]){
            if(i == p || size[i] <= _n/2 || dead[i]) continue;
            return get_cen(i, v);
        }
        return v;
    }
    int find(int v){
        get_size(v, -1);
        _n = size[v];
        return get_cen(v, -1);
    }
    void dfs(int v, int cc){
        int cen = find(v);
        dead[cen] = true;
        for(auto i : g[cen]) if(!dead[i]) dfs(i, cen);
        if(cc >= 0) adj[cc].pb(cen), adj[cen].pb(cc);
        else rt = cen;
    }
    void decomp(){
        dead.assign(SZ, false);
        dfs(0, -1);
    }
};