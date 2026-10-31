/*
 * @lc app=leetcode.cn id=121 lang=cpp
 *
 * [121] 买卖股票的最佳时机
 */

// @lc code=start
class Solution {
public:
    // 状态机DP
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if(n <= 1) return 0;
        int pre0 = 0;
        int pre1 = -prices[0];

        for(int i = 1; i < n; i++){
            int new0 = max(pre0, pre1 + prices[i]);
            int new1 = max(pre1, -prices[i]);
            pre0 = new0;
            pre1 = new1;
        }
        return pre0 > 0 ? pre0 : 0;
    }
};
// @lc code=end

class Solution {
public:
    // 贪心算法
    int maxProfit(vector<int>& prices) {
        // 贪心策略：每次找到更小的就向后更新最大值
        int res = 0;
        int min = prices[0];
        for(int x : prices) {
            if(x < min)
                min = x;  // 更新最低买入点
            else   // 更新最大利润
                res = max(res, x - min);
        }
        return res;
    }
};