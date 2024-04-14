/*  Overview: 
        -  Disjoint set union (union find) data structure
    Parts
        - SZ = # of nodes
        - p[i] = parent of ith node
        - s[i] = size of ith node's connected component
        - r[i] = rank of ith node
        - ONLY UPDATED FOR REPRESENTATIVE ELEMENTS
    Methods:
        - find(u):      Finds representative element of uth node and path 
                        compresses in process. Amortized O(\alpha(SZ))
        - merge(u, v):  Connects components of u and v if not connected already.
                        Uses union by rank heuristic. Amortized O(\alpha(SZ))
    Verification: 
        - https://judge.yosupo.jp/submission/11571       */
struct DSU{
    int SZ;
    vector<int> p, r, s;
    DSU(int sz): SZ(sz){
        p.assign(SZ, 0);
        iota(p.begin(), p.end(), 0);
        r.assign(SZ, 0);
        s.assign(SZ, 1);
    }
    int find(int u){
        if(p[u] != u) p[u] = find(p[u]);
        return p[u];
    }
    void merge(int u, int v){
        u = find(u); v = find(v);
        if(u == v) return;
        if(r[u] < r[v]) swap(u, v);
        if(r[u] == r[v]) ++r[u];
        s[u] += s[v], p[v] = u;
    }
};
