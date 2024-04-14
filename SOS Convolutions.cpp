/* Verification (Or convolution untested): 
        - https://judge.yosupo.jp/submission/67990 (Subset Sum, Zeta, Inverse Zeta)
        - https://judge.yosupo.jp/submission/68000 (AND convolution, Sum over Subsets, Inverse Sum over Subsets)
*/

/*
Zeta Transform and Simple Variants
    Goal: Compute the following transforms in O(N * 2^N)
        - Vanilla Zeta Transform: z(f(S)) := \sum_{S' \subseteq S} f(S')
        - Vanilla Mobius (Inverse Zeta) Transform: \mu(f(S)) := \sum_{S' \subseteq S} -1^{|S \setminus S'|} f(S')
            - Useful because \mu(z(f)) = f and z(\mu(f)) = f
        - Sum over Supersets Transform (Superset Zeta): z'(f(S)) := \sum_{S' \supseteq S} f(S')
        - Inverse Sum over Supersets Transform (Superset Mobius): \mu'(f(S)) := \sum_{S' \supseteq S} -1^{|S \setminus S'|} f(S')
            - Useful because \mu'(z'(f)) = f and z'(\mu'(f)) = f
    Basic Idea: dp[mask][i] = \sum f(k) where k differs from mask in 
    only the i least significant bits and k \subseteq mask
    To save space we overwrite f at ith iteration to replace 
    f_prev[mask] := dp[mask][i - 1] with f_new[mask] := dp[mask][i]
    
    Ops:
        - op1: Superset --> Combine if (mask >> i) & 1 is 0 instead of 1
        - op2: Inverse(Mobius) --> Subtract dp[mask ^ (1 << i)][i - 1] instead of adding
        - Combine-able (any combination of superset/subset, inverse/forward is possible)

    Assumptions/Limitations
        - f is of size 1 << n
        - in place transform
        - (1 << n) <= MAX_INT
*/

template<typename T> 
void zeta(int n, vector<T> &f, bool superset=false, bool inverse=false){
    int op1 = (superset ? 1 : 0), op2 = (inverse ? -1 : 1);
    for(int i = 0; i < n; ++i){
        for(int mask = 0; mask < (1LL << n); ++mask){
            if(((mask >> i) & 1) ^ op1) f[mask] += op2 * f[mask ^ (1 << i)];
        }
    }
}

/*
Subset Sum Convolution
    Goal: Compute the subset sum convolution f \star g (S) for all subsets of (1 << N) - 1 in O(N^2 * 2^N)
        - f \star g (S) := \sum_{S' \subseteq S} f(S') g(S \setminus S')
        - That is, for each set S, we compute the sum of f(A) * g(B) for A \sqcup B = S 
    Basic Idea: Define F(i, S) = f(S) if |S| = i or 0 o.w. Define G(i, S) similarly. Then,
        - f \star g (S) := \mu(\sum_{i = 0}^{|S|} z(F(i, S)) * z(G(|S| - i, S)))
    Assumptions/Limitations
        - f, g are of size 1 << n
        - NOT in place transform
        - (1 << n) <= MAX_INT
*/

template<typename T> 
vector<T> subset_sum_convolution(int n, const vector<T> &f, const vector<T> &g){
    vector<T> res(1 << n);
    vector<vector<T>> conv_f(n + 1, vector<T>(1 << n)), conv_g(n + 1, vector<T>(1 << n)), conv_h(n + 1, vector<T>(1 << n));
    for(int mask = 0; mask < (1 << n); ++mask){
        conv_f[__builtin_popcount(mask)][mask] = f[mask];
        conv_g[__builtin_popcount(mask)][mask] = g[mask];
    }
    for(int i = 0; i <= n; ++i){
        zeta(n, conv_f[i]); zeta(n, conv_g[i]);
    }
    for(int mask = 0; mask < (1 << n); ++mask){
        for(int i = 0; i <= n; ++i){
            for(int j = 0; j <= i; ++j){
                conv_h[i][mask] += conv_f[j][mask] * conv_g[i - j][mask];
            }
        }
    }
    for(int i = 0; i <= n; ++i){
        zeta(n, conv_h[i], false, true);
    }
    for(int mask = 0; mask < (1 << n); ++mask){
        res[mask] = conv_h[__builtin_popcount(mask)][mask];
    }
    return res;
}

/*
Or Convolution
    Goal: Compute the or convolution f \times_{or} g (S) for all subsets of (1 << N) - 1 in O(N * 2^N)
        - f \times_{or} g (S) := \sum_{A \cup B = S} f(A) g(B)
        - That is, for each set S, we compute the sum of f(A) * g(B) for A \cup B = S 
    Basic Idea: Provable with some algebra (apply zeta to both sides)
        - f \times_{or} g = \mu(z(f) * z(g))
    Assumptions/Limitations
        - f, g are of size 1 << n
        - NOT in place transform
        - (1 << n) <= MAX_INT
*/

template<typename T> 
vector<T> or_convolution(int n, vector<T> &f, vector<T> &g){
    vector<T> res(1 << n);
    zeta(n, f); zeta(n, g);
    for(int mask = 0; mask < (1 << n); ++mask){
        res[mask] = f[mask] * g[mask];
    }
    zeta(n, res, false, true);
    return res;
}


/*
And Convolution
    Goal: Compute the and convolution f \times_{and} g (S) for all subsets of (1 << N) - 1 in O(N * 2^N)
        - f \times_{and} g (S) := \sum_{A \cap B = S} f(A) g(B)
        - That is, for each set S, we compute the sum of f(A) * g(B) for A \cap B = S 
    Basic Idea: Provable with some algebra (apply sum over supersets to both sides)
        - f \times_{and} g = \mu'(z'(f) * z'(g))
    Assumptions/Limitations
        - f, g are of size 1 << n
        - NOT in place transform
        - (1 << n) <= MAX_INT
*/

template<typename T> 
vector<T> and_convolution(int n, vector<T> &f, vector<T> &g){
    vector<T> res(1 << n);
    zeta(n, f, true, false); zeta(n, g, true, false);
    for(int mask = 0; mask < (1LL << n); ++mask){
        res[mask] = f[mask] * g[mask];
    }
    zeta(n, res, true, true);
    return res;
}