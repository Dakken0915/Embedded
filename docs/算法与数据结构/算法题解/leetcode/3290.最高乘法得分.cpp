/*
 * @lc app=leetcode.cn id=3290 lang=cpp
 *
 * [3290] 最高乘法得分
 */

// @lc code=start
class Solution {
public:
    long long maxScore(vector<int>& a, vector<int>& b) {
        long long INF = 1e18;
        int m = b.size();
        
        // dp[j][i] 表示b 中前i 项选了j 项的最大得分 
        vector<vector<long long>> dp(5, vector<long long>(m + 1, -INF));
        for(long long& x : dp[0])
            x = 0;

        for(int i = 1; i <= m; i++) {
            long long cur = b[i - 1];
            for(int j = 1; j <= 4; j++) {
                long long val = a[j - 1];
                // 选这项或跳过
                long long select = dp[j - 1][i - 1] + cur * val;
                long long skip = dp[j][i - 1];
                dp[j][i]  = max(select, skip);
            }
        }
        return dp[4][m];
    }
};
// @lc code=end

