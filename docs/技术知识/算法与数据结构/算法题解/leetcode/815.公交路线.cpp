/*
 * @lc app=leetcode.cn id=815 lang=cpp
 *
 * [815] 公交路线
 */

// @lc code=start
class Solution {
public:
    int numBusesToDestination(vector<vector<int>>& routes, int source, int target) {
        if(source == target) return 0;

        int n = routes.size();
        // stop -> 哪些公交经过这个站
        unordered_map<int,vector<int>> stop2bus;
        for(int i = 0; i < n; i++){
            for(int stop : routes[i]){
                stop2bus[stop].push_back(i);
            }
        }

        // 公交与公交建图：有公共站点就相连
        vector<vector<int>> bus_graph(n);
        for(auto& p : stop2bus){
            auto& buses = p.second;
            for(int i = 0; i < buses.size(); i++){
                for(int j = i+1; j < buses.size(); j++){
                    int u = buses[i];
                    int v = buses[j];
                    bus_graph[u].push_back(v);
                    bus_graph[v].push_back(u);
                }
            }
        }

        queue<int> q;
        vector<int> dis(n, -1);
        // 所有经过source的公交入队，坐这一辆，次数为1
        for(int bus : stop2bus[source]){
            dis[bus] = 1;
            q.push(bus);
        }

        while(!q.empty()){
            int x = q.front();
            q.pop();

            // 判断这辆公交能不能到target
            for(int stop : routes[x]){
                if(stop == target){
                    return dis[x];
                }
            }

            // 换乘其他公交
            for(int y : bus_graph[x]){
                if(dis[y] == -1){
                    dis[y] = dis[x] + 1;
                    q.push(y);
                }
            }
        }
        return -1;
    }
};
// @lc code=end

