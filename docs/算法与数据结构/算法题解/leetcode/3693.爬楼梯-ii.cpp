/*
 * @lc app=leetcode.cn id=3693 lang=cpp
 *
 * [3693] 爬楼梯 II
 */

// @lc code=start
class Solution {
public:
    int climbStairs(int n, vector<int>& costs) {
        vector<int> dp(n + 1, INT_MAX);
        dp[0] = 0;
        for(int i = 1; i <= n; i++) {
            for(int j = i - 1; j >= max(i - 3, 0); j--) {
                dp[i] = min(dp[i], costs[i-1]+(i-j)*(i-j)+dp[j]);
            }
        }
        return dp[n];
    }
};
// @lc code=end

