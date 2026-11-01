/*
 * @lc app=leetcode.cn id=797 lang=cpp
 *
 * [797] 所有可能的路径
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> result; // 保存符合条件的所有路径
    vector<int> path; // 起点到终点的路径

    void dfs(vector<vector<int>>& graph, int i, int target) {
        path.push_back(i);
        if(i == target) {
            result.push_back(path); // 保存答案
        }
        
        for(int x : graph[i])
            dfs(graph, x, target);

        path.pop_back();  // 回溯：撤销选择，恢复现场
    }

    vector<vector<int>> allPathsSourceTarget(vector<vector<int>>& graph) {
        int n = graph.size();
        dfs(graph, 0, n - 1);
        return result;
    }
};
// @lc code=end

