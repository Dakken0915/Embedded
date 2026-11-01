/*
 * @lc app=leetcode.cn id=122 lang=cpp
 *
 * [122] 买卖股票的最佳时机 II
 */

// @lc code=start
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        // prev0 表示不持有时的最大利润
        // prev1 表示持有时的最大利润
        int prev0 = 0;
        int prev1 = -prices[0];

        int n = prices.size();
        for(int i = 0; i < n; i++) {
            int cur0 = max(prev0, prev1 + prices[i]);
            // prev0 - prices[i] - 卖出之后再买入
            int cur1 = max(prev1, prev0 - prices[i]);
            prev0 = cur0;
            prev1 = cur1;
        }
        return prev0 < 0 ? 0 : prev0;
    }
};
// @lc code=end

