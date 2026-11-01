/*
 * @lc app=leetcode.cn id=188 lang=cpp
 *
 * [188] 买卖股票的最佳时机 IV
 */

// @lc code=start
class Solution {
public:
    int maxProfit(int k, vector<int>& prices) {
        // dp[i] 代表使用了 i 次交易机会
        vector<vector<int>> dp(k + 1, vector<int>(2, INT_MIN));

        for(int j = 0; j <= k; j ++)
            dp[j][0] = 0;

        for(int p : prices) {
            for(int j = k; j > 0; j--) {
                dp[j][0] = max(dp[j][0], dp[j][1] + p);
                // 在买入的时候使用掉一次交易机会
                dp[j][1] = max(dp[j][1], dp[j - 1][0] - p);
            }
        }
        int ans = dp[k][0];
        return max(ans, 0);
    }
};
// @lc code=end

