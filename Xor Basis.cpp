/*  Overview: 
        - Maintains a basis of Z^{SZ}_2
    Parts
        - SZ = Number of bits required (log_2(MAX))
        - T = Basis Type (LL or int depending on vector size)
        - basis[i] = Unique element in basis with ith bit as lowest bit turned on
    Methods: 
        - insert(T vec):    Inserts a vector into the basis if it is linearly 
                            independent from all other vectors currently in basis. 
                            O(log_2(MAX))
        - find(T vec):      Return true if a vector can be made from the elements 
                            in the basis. O(log_2(MAX))
        - normalize():      Removes redundant on bits (If basis[i] != 0, it is the
                            only basis element with the ith bit on). O(log^2_2(MAX))
    Verification: 
        - insert(), find(): https://atcoder.jp/contests/agc045/submissions/14138170
        - normalize():      N/A        */
template<int SZ, typename T> struct XorBasis{
    int sz;
    T basis[SZ];
    XorBasis(){
        sz = 0;
        memset(basis, 0, sizeof(basis));
    }
    void insert(T vec){
        for(int i = 0; i < SZ; ++i){
            if(!((vec >> i) & 1)) continue;
            if(!basis[i]){
                basis[i] = vec;
                sz++;
                return;
            }
            vec ^= basis[i];
        }
    }
    bool find(T vec){
        for(int i = 0; i < SZ; ++i){
            if(!((vec >> i) & 1)) continue;
            if(!basis[i]){
                return false;
            }
            vec ^= basis[i];
        }
        return true;
    }
    void normalize(){
        for(int i = 0; i < SZ; ++i){
            for(int j = 0; j < i; ++j){
                if(basis[i] && (basis[j] >> i) & 1){
                    basis[j] ^= basis[i];
                }
            }
        }
    }
};