/*
 * @lc app=leetcode.cn id=743 lang=cpp
 *
 * [743] 网络延迟时间
 */

// @lc code=start
class Solution {
public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        vector<vector<pair<int, int>>> graph(n);  // 邻接表
        for(auto &arr : times){
            // 对每个节点做偏移
            int source = arr[0] - 1;
            int target = arr[1] - 1;
            int weight = arr[2];
            graph[source].push_back({weight, target});
        }

        vector<int> dis(n, INT_MAX);
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> minHeap; 
        dis[k - 1] = 0;
        minHeap.push({0, k - 1});

        while(!minHeap.empty()) {
            auto [dis_x, x] = minHeap.top();
            minHeap.pop();
            
            // 有更小的出现过
            if(dis_x > dis[x]) continue;

            // 更新 x 邻居的最短路
            for(auto& [weight, y] : graph[x]) {
                if(dis_x + weight < dis[y]) {
                    dis[y] = dis_x + weight;
                    minHeap.push({dis[y], y});
                }
            }
        }
        int res = 0;
        for(int x : dis) {
            if(x == INT_MAX) return -1;
            res = max(res, x);
        }
        return res;
    }
};
// @lc code=end

