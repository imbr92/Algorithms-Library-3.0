/*  Overview: 
        - Dijkstra's Algorithm (Shortest path for directed or 
          undirected graphs w/ non-negative edge weights)
    Parts:
        - T = Edge weight type
        - dist[] = vector to put resulting shortest paths in
        - adj[i][j] = jth connection of ith node {node, edge weight}
        - INF = Infinity (> max length)
    Complexity: 
        - O(E log_2(V))
    Verification: 
        - https://cses.fi/problemset/result/621148/     */
template<typename T> void dijkstra(int src, vector<T> &dist, vector<vector<pair<int, T>>> &adj, T INF){
    priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
    dist.assign(n, INF); pq.push({0, src}); dist[src] = 0;
    while(!pq.empty()){
        pair<T, int> cur = pq.top(); pq.pop();
        if(dist[cur.s] < cur.f) continue;
        for(auto &i : adj[cur.s]){
            if(dist[i.f] > dist[cur.s] + i.s){
                dist[i.f] = dist[cur.s] + i.s;
                pq.push({dist[i.f], i.f});
            }
        }
    }
}