#include "bits/stdc++.h"

using namespace std;

typedef long double ld;
typedef long long ll;

#define sz(x) (int)(x).size()
#define pb push_back
#define mp make_pair
#define f first
#define s second

template<typename T, typename U> bool ckmin(T &a, U b){ return b < a ? a = b, true : false; }
template<typename T, typename U> bool ckmax(T &a, U b){ return b > a ? a = b, true : false; }

namespace Factorization{
    struct FactorizationSieve{
	vector<int> s;
	FactorizationSieve(int MAX_NUM){
	    s.assign(MAX_NUM + 5, 1);
	    s[0] = 0;
	    for(int i = 2; i <= MAX_NUM; ++i){
		if(s[i] != 1){ // not prime --> skip
		    continue;
		}
		for(int j = i; j <= MAX_NUM; j += i){
		    if(s[j] == 1) s[j] = i;
		}
	    }
	}
	
	vector<pair<int, int>> get_prime_factors(int x){
	    map<int, int> pfs;
	    while(x != 1){
		++pfs[s[x]];
		x /= s[x];
	    }
	    return vector<pair<int, int>>(pfs.begin(), pfs.end());
	}

	vector<int> get_factors(int x){
	    auto pfs = get_prime_factors(x);
	    vector<int> ret;
	    gen(0, pfs, ret, 1);
	    return ret;
	}

	private:
	    void gen(int idx, const vector<pair<int, int>> &pfs, vector<int> &ret, int cur){
		if(idx == sz(pfs)){
		    ret.push_back(cur);
		    return;
		}
		int mul = 1;
		for(int i = 0; i <= pfs[idx].s; ++i){
		    gen(idx + 1, pfs, ret, cur * mul);
		    mul *= pfs[idx].f;
		}
	    }
    };

    vector<pair<int, int>> get_prime_factors(int x){
	map<int, int> pfs;
	ll cur = 2;
	while(x > 1 && cur * cur <= x){
	    while(x % cur == 0){
		++pfs[cur];
		x /= cur;
	    }
	    ++cur;
	}
	if(x > 1) ++pfs[x];
	return vector<pair<int, int>>(pfs.begin(), pfs.end());
    }

    vector<int> get_factors(int x){
	vector<int> facs;
	ll cur = 2;
	while(cur * cur <= x){
	    if(x % cur == 0){
		facs.push_back(cur);
		if(x / cur != cur) facs.push_back(x / cur);
	    }
	    ++cur;
	}
	sort(facs.begin(), facs.end());
	return facs;
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    return 0;
}
