/*
 * @lc app=leetcode.cn id=1971 lang=cpp
 *
 * [1971] 寻找图中是否存在路径
 */

// @lc code=start
class Solution {
public:
    int dfs(vector<vector<int>>& g, vector<bool>& vis, int source, int target) {
        vis[source] = true;  // 标记已访问
        if(source == target) return 1;  // 找到了目标值
        int res = 0;
        for(auto& x : g[source]) {
            if(!vis[x])
                res += dfs(g, vis, x, target);
        }
        return res;
    }

    bool validPath(int n, vector<vector<int>>& edges, int source, int destination) {
        vector<vector<int>> g(n);

        // 所有边沿加入无向图
        for(auto& edge : edges) {
            int x = edge[0];
            int y = edge[1];
            g[x].push_back(y);
            g[y].push_back(x);
        }

        vector<bool> vis(n, false);  // 初始未访问
        
        return dfs(g, vis, source, destination) ? true : false;
    }
};
// @lc code=end

