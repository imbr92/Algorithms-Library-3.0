#include <bits/stdc++.h>

using namespace std;

template<typename T, typename U> bool ckmax(T &a, U b){ return b > a ? a = b, true : false; }

/* Call get_diameter(adj) on adjacency list of unweighted tree to get a diameter with endpoints u, v and length = dist*/
namespace Diameter {
    namespace detail{
        // dist, vertex
        pair<int, int> find_furthest(int v, int p, int cur_dist, const vector<vector<int>> &adj){
            pair<int, int> best = {cur_dist, v};
            for(auto i : adj[v]){
                if(i == p) continue;
                ckmax(best, find_furthest(i, v, cur_dist + 1, adj));
            }
            return best;
        }
    }

    struct path{
        int u, v, dist;
    };

    path get_diameter(const vector<vector<int>> &adj){
        auto [_, u] = detail::find_furthest(0, -1, 0, adj);
        auto [dist, v] = detail::find_furthest(u, -1, 0, adj);

        return {.u = u, .v = v, .dist = dist};
    }
};

int main(){
    return 0;
}
