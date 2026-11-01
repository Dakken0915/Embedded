/*
 * @lc app=leetcode.cn id=3243 lang=cpp
 *
 * [3243] 新增道路查询后的最短距离 I
 */

// @lc code=start
class Solution {
public:
    int bfs(vector<vector<int>>& graph) {
        queue<int> que;
        que.push(0);

        vector<int> dis(graph.size(), -1);  // 表示尚未到达
        dis[0] = 0;

        while(!que.empty()) {
            int x = que.front();
            que.pop();
            for(int city : graph[x]) {
                // BFS, 路径短的一定先到达，只更新第一次就可以
                if(dis[city] == -1) { // 第一次访问，得到最短距离
                    dis[city] = dis[x] + 1;
                    que.push(city);
                }
            }
        }
        return dis[graph.size() - 1];
    }

    vector<int> shortestDistanceAfterQueries(int n, vector<vector<int>>& queries) {
        vector<vector<int>> graph(n);
        // 初始化路径
        for(int i = 0; i < n - 1; i++)
            graph[i].push_back(i + 1);
        
        vector<int> res;
        int m = queries.size();
        for(int i = 0; i < m; i++) {
            // 更新路径
            int start = queries[i][0];
            int end = queries[i][1];
            graph[start].push_back(end);

            //计算最短路径
            int len = bfs(graph);
            res.push_back(len);
        }
        return res;
    }
};
// @lc code=end

