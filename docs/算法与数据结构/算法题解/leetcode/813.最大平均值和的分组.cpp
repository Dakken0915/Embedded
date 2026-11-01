/*
 * @lc app=leetcode.cn id=813 lang=cpp
 *
 * [813] 最大平均值和的分组
 */

// @lc code=start
class Solution {
public:
    double largestSumOfAverages(vector<int>& nums, int k) {
        int n = nums.size();

        // 计算前缀和
        vector<int> prev(n + 1, 0);
        for(int i = 1; i <= n; i++) {
            prev[i] = prev[i - 1] + nums[i - 1];
        }

        // dp[i][m]：前i个元素恰好分成m段
        vector<vector<double>> dp(n + 1, vector<double>(k + 1, -1));
        dp[0][0] = 0;

        // 初始化不分段的情况
        for(int i = 1; i <= n; i++) {
            dp[i][1] = (double)prev[i] / i;
        }

        for(int m = 2; m <= k; m++) {
            for(int i = m; i <= n; i++) {
                for(int j = m - 1; j < i; j++) {
                    // 将 j - i 视为一段
                    double ave =  (double)(prev[i] - prev[j]) / (i - j);
                    dp[i][m] = max(dp[i][m], dp[j][m - 1] + ave);
                }
            }
        }
        // 只看前n个元素，分1~k段
        double res = -1e18;
        for(int t = 1; t <= k; t++)
            res = max(res, dp[n][t]);
            
        return res;
    }
};
// @lc code=end

