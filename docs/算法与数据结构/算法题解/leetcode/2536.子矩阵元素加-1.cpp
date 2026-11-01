/*
 * @lc app=leetcode.cn id=2536 lang=cpp
 *
 * [2536] 子矩阵元素加 1
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> rangeAddQueries(int n, vector<vector<int>>& queries) {
        vector diff(n + 2, vector<int>(n + 2));
        for(int i = 0; i < queries.size(); i++) {
            int x1 = queries[i][0];
            int y1 = queries[i][1];
            int x2 = queries[i][2];
            int y2 = queries[i][3];
            diff[x1 + 1][y1 + 1]++;
            diff[x1 + 1][y2 + 2]--;
            diff[x2 + 2][y1 + 1]--;
            diff[x2 + 2][y2 + 2]++;
        }
        // 原地计算 diff 的二维前缀和，然后填入答案
        vector ans(n, vector<int>(n));
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < n; j++) {
                diff[i + 1][j + 1] += diff[i + 1][j] + diff[i][j + 1] - diff[i][j];
                ans[i][j] = diff[i + 1][j + 1];
            }
        }
        return ans;
    }
};
// @lc code=end

