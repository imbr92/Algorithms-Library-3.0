/*  Overview: 
        - Standard polynomial Rolling Hash (Single modulo)
    Parts
        - MOD = inputted modulus, try ~1e9
        - al = alphabet size
        - off = offset (usually 'a')
        - ipw[i] = al^{-i}
    Methods:
        - hash(str):    Returns a vector containing prefix polynomial hashes
                        of the given string
        - qry(l, r):    Returns hash of substring [l, r]
    Verification: 
        - https://judge.yosupo.jp/submission/13121       */

struct rolling_hash{
    ll MOD, al, off, i_al = -1;
    vector<ll> ipw;
    
    rolling_hash(ll MOD, ll al, ll off): MOD(MOD), al(al), off(off) {}
    
    ll exp(ll a, ll r){
        if(r == 0) return 1;
        ll tmp = exp(a, r/2);
        tmp = (tmp * tmp)%MOD;
        if(r & 1) tmp = (tmp * a)%MOD;
        return tmp;
    }

    void upd(int n){
        if(i_al == -1){
            i_al = exp(al, MOD - 2);
            ipw.pb(1);
        }
        while(sz(ipw) <= n) ipw.pb((ipw.back() * i_al)%MOD);
    }

    template<typename T> vector<ll> hash(T &str){
        vector<ll> v(sz(str) + 1);
        upd(sz(str) + 5);
        ll nxt = 0, pw = 1;
        for(int i = 0; i < sz(str); ++i){
            v[i + 1] = (nxt + (str[i] - off + 1) * pw)%MOD;
            nxt = v[i + 1];
            pw = (pw * al)%MOD;
        }
        return v;
    }

    ll qry(int l, int r, vector<ll> &v){
        ll res = v[r + 1] - v[l];
        return (((ipw[l] * res)%MOD) + MOD)%MOD;
    }
};