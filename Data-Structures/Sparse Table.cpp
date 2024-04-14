/*  Overview: 
        - Sparse table data structure (default range min)
    Parts
        - SZ = # of elements in original array
        - table[i][j] = binary operator on all values from [j, j + (1 << i))
        CHANGE cmb() TO CHANGE OPERATION (must be idempotent)
    Methods:
        - cmb(x, y):    Returns combined value of x and y (performs segtree 
                        binary operation). O(1)
        - build():      Builds initial sparse table. O(SZ log_2(SZ))
        - qry(l, r):    Returns value of binary operation over range [l, r].
                        O(1)
    Verification: 
        - https://judge.yosupo.jp/submission/14360       */
template<typename T> struct SparseTable{
    int SZ, L;
    vector<int> logs;
    vector<vector<T>> table;
    T cmb(T x, T y){ return min(x, y); }
    SparseTable(T id, int size, vector<T> &v): SZ(size){
        L = log2(SZ) + 1;
        logs.assign(SZ + 1, 0);
        for(int i = 2; i <= SZ; ++i) logs[i] = logs[i >> 1] + 1;
        table.assign(L + 1, vector<T>(SZ, id));
        build(v);
    }
    void build(vector<T> &v){
        for(int i = 0; i < SZ; ++i) table[0][i] = v[i];
        for(int i = 1; i <= L; ++i){
            for(int j = 0; j < SZ; ++j){
                table[i][j] = cmb(table[i - 1][j], table[i - 1][min(j + (1 << (i - 1)), SZ - 1)]);
            }
        }
    }
    T qry(int l, int r){
        int j = logs[r - l + 1];
        return cmb(table[j][l], table[j][r - (1 << j) + 1]);
    }
};
