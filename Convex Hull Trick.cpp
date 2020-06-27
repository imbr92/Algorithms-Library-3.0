/*  Overview: 
        - Convex Hull Trick Data Structure (Line Container). Query max at x, Add line to envelope
    Parts
        - INF = Infinity for datatype
    Methods:
        - div(a, b):    Floored division with support for negatives. O(1)
        - isect(x, y):  Helper function for line intersection. O(1)
        - add(m, b):    Adds y = mx + b to container. O(log_2(N))
        - qry(x):       Returns max value of all lines evaluated at x. O(log_2(N))
    Modification: 
        - Using doubles/ld --> Change INF to 1/.0 and div to a / b.
        - Querying mins (upper envelope or lower hull) --> Insert 
          lines (-m, -b) and -qry(x) is y.
    Source: KACTL
    Verification: 
        - https://judge.yosupo.jp/submission/13991       */
struct Line{
    mutable ll m, b, p;
    bool operator<(const Line& l) const { return m < l.m; }
    bool operator<(ll x) const { return p < x; } 
};


struct LineContainer : multiset<Line, less<>> {
    const ll INF = INT64_MAX;
    ll div(ll a, ll b){ // fast floored division
        return a / b - ((a ^ b) < 0 && a % b);
    }
    bool isect(iterator x, iterator y){
        if(y == end()){ 
            x->p = INF; 
            return false; 
        }
        if(x->m == y->m) x->p = (x->b > y->b ? INF : -INF);
        else x->p = div(y->b - x->b, x->m - y->m);
        return x->p >= y->p;
    }
    void add(ll m, ll b){
        auto z = insert({m, b, 0}), y = z++, x = y;
        while(isect(y, z)) z = erase(z);
        if(x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while((y = x) != begin() && (--x)->p >= y->p){
            isect(x, erase(y));
        }
    }
    ll qry(ll x){
        assert(!empty());
        auto l = *lower_bound(x);
        return l.m * x + l.b;
    }
};