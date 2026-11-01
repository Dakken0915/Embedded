/*
 * @lc app=leetcode.cn id=123 lang=cpp
 *
 * [123] 买卖股票的最佳时机 III
 */

// @lc code=start
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        vector<vector<int>> dp(3, vector<int>(2, INT_MIN));

        for(int i = 0; i <= 2; i++) {
            dp[i][0] = 0;
        }

        for(int price : prices) {
            for(int j = 2; j > 0; j--) {
                dp[j][0] = max(dp[j][0], dp[j][1] + price);
                dp[j][1] = max(dp[j][1], dp[j - 1][0] - price);
            }
        }
        return max(dp[2][0], 0);
    }
};
// @lc code=end

