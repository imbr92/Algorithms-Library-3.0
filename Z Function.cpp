/*  Overview: 
        - Z function algorithm (returns z[] where z[i] = LCP b/w s[i, n] and s)
    Parts:
        - T = Argument type (usually string)
    Complexity: 
        - O(n)
    Verification: 
        - https://judge.yosupo.jp/submission/17094      */
template<typename T> vector<int> calc(T &str){
    int n = sz(str);
    vector<int> z(n);
    for(int i = 1, l = 0, r = 0; i < n; ++i){
        z[i] = max(min(r - i, z[i - l]), 0);
        while(i + z[i] < n && str[z[i]] == str[i + z[i]]){
            ++z[i];
        }
        if(i + z[i] > r){
            l = i, r = i + z[i];
        }
    }
    if(n) z[0] = n;
    return z;
}