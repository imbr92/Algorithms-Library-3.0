/*  Overview: 
        - Segment tree data structure (default range sum)
    Parts
        - SZ = # of leaves
        - t[i] = ith node of the tree (ordered like heap)
        - id = Identity element
        CHANGE cmb() TO CHANGE OPERATION
    Methods:
        - cmb(x, y):    Returns combined value of x and y (performs segtree 
                        binary operation). O(SZ)
        - build():      Builds initial segment tree. O(SZ)
        - qry(l, r):    Returns value of binary operation over range [l, r].
                        O(log_2(SZ))
        - upd(u, x):    Changes uth leaf to x. O(log_2(SZ))
        - chng(u, x):   Add x to the value of the uth leaf. O(log_2(SZ))
    Verification: 
        - https://judge.yosupo.jp/submission/11619       */
template<typename T> struct SegmentTree{
    int SZ;
    T id;
    vector<T> t;
    SegmentTree(T x, int sz): id(x), SZ(sz) {
        t.assign((SZ << 2) + 5, 0);
    }
    SegmentTree(vector<T> &a, T x, int sz){
        id = x; SZ = sz;
        t.assign((SZ << 2) + 5, 0);
        build(1, 0, SZ - 1, a);
    }
    T cmb(T x, T y){ return x + y;}
    void build(int v, int tl, int tr, vector<T> &a){
        if(tl == tr) t[v] = a[tl];
        else{
            int tm = tl + (tr - tl)/2;
            build(v << 1, tl, tm, a);
            build((v << 1) | 1, tm + 1, tr, a);
            t[v] = cmb(t[v << 1], t[(v << 1) | 1]);
        }
    }
    T qry(int l, int r){ return qry(1, 0, SZ - 1, l, r);}
    T qry(int v, int tl, int tr, int l, int r){
        if(l > r) return id;
        if(tl == l && tr == r) return t[v];
        int tm = tl + (tr - tl)/2;
        return cmb(qry(v << 1, tl, tm, l, min(r, tm)), qry((v << 1) | 1, tm + 1, tr, max(l, tm + 1), r));
    }
    void upd(int u, T x){ upd(1, 0, SZ - 1, u, x);}
    void upd(int v, int tl, int tr, int u, T x){
        if(tl == tr) t[v] = x;
        else{
            int tm = tl + (tr - tl)/2;
            if(u > tm) upd((v << 1) | 1, tm + 1, tr, u, x);
            else upd(v << 1, tl, tm, u, x);
            t[v] = cmb(t[v << 1], t[(v << 1) | 1]);
        }
    }
    void chng(int u, T x){ chng(1, 0, SZ - 1, u, x);}
    void chng(int v, int tl, int tr, int u, T x){
        if(tl == tr) t[v] = cmb(t[v], x);
        else{
            int tm = tl + (tr - tl)/2;
            if(u > tm) chng((v << 1) | 1, tm + 1, tr, u, x);
            else chng(v << 1, tl, tm, u, x);
            t[v] = cmb(t[v << 1], t[(v << 1) | 1]);
        }
    }
};