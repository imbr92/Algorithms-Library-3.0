/*  Overview: 
        - Suffix Array (Sort suffixes of a string in O(SZ log_2(SZ)) and 
          query longest common prefixes between suffixes in O(1)) after 
          O(SZ log_2(SZ)) preprocessing.
    Parts
        - T, S = Object to find suffixes of, single element type (usually string, char);
        - minimal = Any element that is guaranteed to be leq all other elements in the string
        - c[i] = Position in ord[] that the suffix starting at character i is at
        - ord[i] = ith smallest prefix of string. 0th smallest is just the minimal element that was appended w/ constructor
        - lcp[i] = Longest common prefix between the ith and i+1th elements of ord[]
        - st = Minimum sparse table of lcp[]
        CHANGE st to min segment tree with small memory limits for build_lcp() in O(SZ) and qry(i, j) in O(log_2(SZ)).
        Important things: Print out arrays before using because indexing is non-standard
    Methods:
        - count_sort():     Counting sort ord. Used as a helper function. O(SZ)
        - build_ord():      Generates suffix array (ord[]) from the given string. O(SZ log_2(SZ))
        - build_lcp():      Generates lcp[] using Kasai, Lee, Arimura, Arikawa, and Park linear 
                            LCP algorithm. O(SZ log SZ)
        - qry(i, j):        Returns LCP between suffix str[i, SZ - 1] and str[j, SZ - 1]. O(1)
    Verification: 
        - https://codeforces.com/edu/course/2/lesson/2/5/practice/contest/269656/submission/85568680
        - https://codeforces.com/edu/course/2/lesson/2/5/practice/contest/269656/submission/85728550
        - https://codeforces.com/edu/course/2/lesson/2/5/practice/contest/269656/submission/85789969
        - https://codeforces.com/edu/course/2/lesson/2/5/practice/contest/269656/submission/85801898        */

// Sparse Table included for convenience (same as "Sparse Table.cpp")
template<typename T> struct SparseTable{
    int SZ, L;
    vector<int> logs;
    vector<vector<T>> table;
    T cmb(T x, T y){ return min(x, y); }
    SparseTable(){};
    SparseTable(int size, vector<T> &v): SZ(size){
        L = log2(SZ) + 1;
        logs.assign(SZ + 1, 0);
        for(int i = 2; i <= SZ; ++i) logs[i] = logs[i >> 1] + 1;
        table.assign(L + 1, vector<T>(SZ, 0));
        build(v);
    }
    void build(int size, vector<T> &v){
        SZ = size;
        L = log2(SZ) + 1;
        logs.assign(SZ + 1, 0);
        for(int i = 2; i <= SZ; ++i) logs[i] = logs[i >> 1] + 1;
        table.assign(L + 1, vector<T>(SZ, 0));
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

template<typename T, typename S> struct SuffixArray{
    int SZ; T str;
    vector<int> lcp, c, ord;
    SparseTable<int> st;
    SuffixArray() {};
    SuffixArray(T s, S minimal, int size): str(s), SZ(size + 1), st(){
        str.pb(minimal);
    }
    void count_sort(){
        vector<int> cnt(SZ), pos(SZ), ord_new(SZ);
        for(int i = 0; i < SZ; ++i) cnt[c[i]]++;
        for(int i = 1; i < SZ; ++i) pos[i] = pos[i - 1] + cnt[i - 1];
        for(int i = 0; i < SZ; ++i){
            ord_new[pos[c[ord[i]]]] = ord[i];
            ++pos[c[ord[i]]];
        }
        ord = ord_new;
    }
    void build_ord(T s, S minimal, int size){
        str = s; SZ = size + 1; str.pb(minimal);
        build_ord();
    }
    void build_ord(){
        ord.assign(SZ, 0); c.assign(SZ, 0);
        int k = 0;
        vector<pair<S, int>> pre(SZ);
        for(int i = 0; i < SZ; ++i) pre[i] = {str[i], i};
        sort(pre.begin(), pre.end());
        for(int i = 0; i < SZ; ++i) ord[i] = pre[i].s;
        c[ord[0]] = 0;
        for(int i = 1; i < SZ; ++i){
            if(pre[i].f == pre[i - 1].f) c[ord[i]] = c[ord[i - 1]];
            else c[ord[i]] = c[ord[i - 1]] + 1;
        }
        while((1 << k) < SZ){
            for(int i = 0; i < SZ; ++i){
                ord[i] = (ord[i] - (1 << k) + SZ)%SZ;
            }
            count_sort();
            vector<int> c_new(SZ);
            c[ord[0]] = 0;
            for(int i = 1; i < SZ; ++i){
                pair<int, int> prev = {c[ord[i - 1]], c[(ord[i - 1] + (1 << k))%SZ]};
                pair<int, int> cur = {c[ord[i]], c[(ord[i] + (1 << k))%SZ]};
                if(prev == cur) c_new[ord[i]] = c_new[ord[i - 1]];
                else c_new[ord[i]] = c_new[ord[i - 1]] + 1;
            }   
            c = c_new; ++k;
        }
    }
    void build_lcp(){
        int k = 0;
        lcp.assign(SZ, 0);
        for(int i = 0; i < SZ - 1; ++i){
            int prev = ord[c[i] - 1];
            while(str[i + k] == str[prev + k]) ++k;
            lcp[c[i]] = k;
            k = max(k - 1, 0);
        }
        st.build(SZ, lcp);
    }
    // //common characters between suffix i and suffix j
    int qry(int i, int j){
        if(i == j) return SZ - i;
        i = c[i], j = c[j];
        if(i > j) swap(i, j);
        return st.qry(i + 1, j);
    }
    void print(){
        cout << "Suffix Array: ";
        for(auto i : ord) cout << i << " ";
        cout << "\nLCP Array: ";
        for(auto i : lcp) cout << i << " ";
        cout << "\nInverse Permutation: ";
        for(auto i : c) cout << i << " ";
        cout << "\n";
    }
};