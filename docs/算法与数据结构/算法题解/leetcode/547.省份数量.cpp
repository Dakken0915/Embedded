/*
 * @lc app=leetcode.cn id=547 lang=cpp
 *
 * [547] 省份数量
 */

// @lc code=start
class Solution {
private:
    void dfs(vector<vector<int>>& isConnected, int x) {
        // 遍历所有城市i，找和x连通的城市
        for(int i = 0; i < isConnected[0].size(); i++) {
            if(isConnected[x][i] == 1) {
                isConnected[x][i] = 0; // 标记已访问
                dfs(isConnected, i);    // 递归访问连通城市i
            }
        }
    }

public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        int res = 0;
        for(int i = 0; i < isConnected.size(); i++) {
            if(isConnected[i][i] == 1) {
                dfs(isConnected, i);
                res++;
            }
        }
        return res;
    }
};
// @lc code=end

