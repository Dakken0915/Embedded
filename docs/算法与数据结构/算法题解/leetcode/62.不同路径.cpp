/*
 * @lc app=leetcode.cn id=62 lang=cpp
 *
 * [62] 不同路径
 */

// @lc code=start

// 空间优化版本
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<int> dp(n, 0);
        dp[0] = 1;

        for(int i = 0; i < m; i++)
            for(int j = 1; j < n; j++)
                dp[j] += dp[j - 1];

        return dp[n - 1];
    }
};
// @lc code=end

class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<vector<int>> dp(m, vector<int>(n, 0));
        // 初始化边沿
        for(int i = 0; i < m; i++)
            dp[i][0] = 1;
        for(int i = 1; i < n; i++)
            dp[0][i] = 1;

        for(int i = 1; i < m; i++)
            for(int j = 1; j < n; j++)
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];

        return dp[m -1][n - 1];
    }
};


