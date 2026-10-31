/*
 * @lc app=leetcode.cn id=3665 lang=cpp
 *
 * [3665] 统计镜子反射路径数目
 */

// @lc code=start
class Solution {
public:
    int uniquePaths(vector<vector<int>>& grid) {
        const int MOD = 1e9 + 7;
        int m = grid.size();
        int n = grid[0].size();

        vector<vector<int>> dp(n + 1, vector<int>(2, 0));
        dp[1][0] = 1;
        dp[1][1] = 1;
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if(grid[i][j] == 0) {
                    // 上方的下侧输出和左方的右侧输出
                    dp[j + 1][0] = (dp[j][0] + dp[j + 1][1]) % MOD;
                    dp[j + 1][1] = dp[j + 1][0];
                }
                else {
                    dp[j + 1][0] = dp[j + 1][1];  // 输出给右侧的来自上方
                    dp[j + 1][1] = dp[j][0];  // 输出给下方的来自左侧
                }
            }
        }
        return dp[n][0];
    }
};
// @lc code=end

class Solution {
public:
    // touch
    int uniquePaths(vector<vector<int>>& grid) {
        const int MOD = 1e9 + 7;
        int n = grid[0].size();
        vector<array<int, 2>> f(n + 1);
        f[1] = {1, 1}; // 代表到达起点 (0,0) 时，两种入射方向下路径数都为 1
        for (auto& row : grid) {
            for (int j = 0; j < n; j++) {
                if (row[j] == 0) {
                    // [0] 代表右侧出口 [1] 代表下方出口
                    f[j + 1][0] = (f[j][0] + f[j + 1][1]) % MOD;
                    f[j + 1][1] = f[j + 1][0];
                } else {
                    f[j + 1][0] = f[j + 1][1];
                    f[j + 1][1] = f[j][0];
                }
            }
        }
        return f[n][0];
    }
};
