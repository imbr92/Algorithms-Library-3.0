/*  Overview: 
        - Lazy segment tree data structure (default range max queries, range sum updates)
    Parts
        - SZ = # of leaves
        - t[i] = ith node of the tree (ordered like heap)
        - id = Identity element
        CHANGE cmb() TO CHANGE OPERATION
    Methods:
        - cmb(x, y):        Returns combined value of x and y (performs segtree 
                            binary operation). O(SZ)
        - build():          Builds initial segment tree. O(SZ)
        - qry(l, r):        Returns value of binary operation over range [l, r].
                            O(log_2(SZ))
        - upd(l, r, x):     Applies lazy update by x to [l, r]. O(log_2(SZ))
        - push(v, tl, tr):  Pushes lazy operations down the tree. Takes in tl and tr
                            in case the push requires using number of elements covered by
                            that node. O(1)
    Verification: 
        - https://codeforces.com/contest/1371/submission/85786163 
        - https://codeforces.com/contest/1373/submission/85493826
        - https://judge.yosupo.jp/submission/14244       */

template<typename T, typename S> struct LazySegmentTree{
    int SZ;
    T id;
    vector<T> t;
    vector<S> lazy;
    LazySegmentTree(T x, int sz): id(x), SZ(sz), lazy((SZ << 2) + 5), t((SZ << 2) + 5) {}
    LazySegmentTree(vector<T> &a, T x, int sz): id(x), SZ(sz), lazy((SZ << 2) + 5), t((SZ << 2) + 5){
        build(1, 0, SZ - 1, a);
    }
    void push(int v, int tl, int tr){
        int tm = tl + (tr - tl)/2;
        if((v << 1) < (SZ << 2) + 5){
            lazy[v << 1] += lazy[v];
            t[v << 1] += lazy[v];
        }
        if(((v << 1) | 1) < (SZ << 2) + 5){
            lazy[(v << 1) | 1] += lazy[v];
            t[(v << 1) | 1] += lazy[v];
        }
        lazy[v] = 0;
    }
    T cmb(T x, T y){ return max(x, y);}
    void build(int v, int tl, int tr, vector<T> &a){
        if(tl == tr) t[v] = a[tl];
        else{
            int tm = tl + (tr - tl)/2;
            build(v << 1, tl, tm, a);
            build((v << 1) | 1, tm + 1, tr, a);
            t[v] = cmb(t[v << 1], t[(v << 1) | 1]);
        }
    }
    T qry(int l, int r){ return qry(1, 0, SZ - 1, l, r);};
    T qry(int v, int tl, int tr, int l, int r){
        push(v, tl, tr);
        if(l > r) return id;
        int tm = tl + (tr - tl)/2;
        if(tl == l && tr == r) return t[v];
        return cmb(qry(v << 1, tl, tm, l, min(r, tm)), qry((v << 1) | 1, tm + 1, tr, max(l, tm + 1), r));
    }
    void upd(int l, int r, T x){ upd(1, 0, SZ - 1, l, r, x);};
    void upd(int v, int tl, int tr, int l, int r, T x){
        push(v, tl, tr);
        if(l > r) return;
        int tm = tl + (tr - tl)/2;
        if(tl == l && tr == r){
            //change lazy op based on lazy
            lazy[v] += x;
            t[v] += x;
        }
        else{
            upd(v << 1, tl, tm, l, min(r, tm), x);
            upd((v << 1) | 1, tm + 1, tr, max(l, tm + 1), r, x);
            t[v] = cmb(t[v << 1], t[(v << 1) | 1]);
        }
    }
};