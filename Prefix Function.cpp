/*  Overview: 
        - Prefix function algorithm (returns pi[] where pi[i] = Longest prefix of s which is a suffix of s[0, i])
    Parts:
        - T = Argument type (usually string)
    Complexity: 
        - O(n)
    Verification: 
        - https://codeforces.com/contest/808/submission/84342787      */

template<typename T> vector<int> prefix_function(T &str){
    int n = sz(str);
    vector<int> pi(n, 0);
    for(int i = 1; i < n; ++i){
        int j = pi[i - 1];
        while(j > 0 && str[i] != str[j]){
            j = pi[j - 1];
        }
        if(str[i] == str[j]) j++;
        pi[i] = j;
    }
    return pi;
}
