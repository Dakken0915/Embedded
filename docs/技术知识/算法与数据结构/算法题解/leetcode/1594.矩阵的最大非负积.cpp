/*
 * @lc app=leetcode.cn id=1594 lang=cpp
 *
 * [1594] 矩阵的最大非负积
 */

// @lc code=start
class Solution {
public:
    int maxProductPath(vector<vector<int>>& grid) {
        int MOD = 1e9 + 7;
        int m = grid.size();
        int n = grid[0].size();

        // 同时记录乘积最大值和最小值
        vector<vector<long long>> dpMax(m, vector<long long>(n, INT_MIN));
        vector<vector<long long>> dpMin(m, vector<long long>(n, INT_MAX));
        dpMax[0][0] = grid[0][0];
        dpMin[0][0] = grid[0][0];
        for(int i = 1; i < m; i++) {
            dpMax[i][0] = grid[i][0] * dpMax[i - 1][0];
            dpMin[i][0] = dpMax[i][0];
        }

        for(int i = 1; i < n; i++) {
            dpMax[0][i] = grid[0][i] * dpMax[0][i - 1];
            dpMin[0][i] = dpMax[0][i];
        }

        for(int i = 1; i < m; i++) {
            for(int j = 1; j < n; j++) {
                dpMax[i][j] = max(dpMax[i - 1][j] * grid[i][j], max(dpMax[i][j - 1] * grid[i][j], 
                max(dpMin[i - 1][j] * grid[i][j], dpMin[i][j - 1] * grid[i][j])));
                dpMin[i][j] = min(dpMax[i - 1][j] * grid[i][j], min(dpMax[i][j - 1] * grid[i][j], 
                min(dpMin[i - 1][j] * grid[i][j], dpMin[i][j - 1] * grid[i][j])));
            }
        }
        if(dpMax[m - 1][n- 1] < 0) return -1;
        return dpMax[m - 1][n- 1] % MOD;
    }
};
// @lc code=end

