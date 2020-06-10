/*  Overview: 
        - Standard Z function algorithm (returns z[] where z[i] = LCP b/w s[i, n] and s)
    Parts:
        - T = Argument type (usually string)
    Complexity: 
        - O(n)
    Verification: 
        - https://judge.yosupo.jp/submission/11658      */
template<typename T> vector<int> calc(T &s) {
    int n = sz(s);
    vector<int> z(n);
    for (int i = 0, l = 0, r = 0; i < n; ++i) {
        if (i <= r)
            z[i] = min (r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            ++z[i];
        if (i + z[i] - 1 > r)
            l = i, r = i + z[i] - 1;
    }
    return z;
}