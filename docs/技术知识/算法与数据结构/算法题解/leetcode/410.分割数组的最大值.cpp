/*
 * @lc app=leetcode.cn id=410 lang=cpp
 *
 * [410] 分割数组的最大值
 */

// @lc code=start
class Solution {
public:
    int splitArray(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> prevSum(n + 1, 0);
        for(int i = 1; i <= n; i++) {
            prevSum[i] = prevSum[i - 1] + nums[i - 1];
        }

        vector<vector<int>> dp(n + 1, vector<int>(k + 1, INT_MAX));
        for(int i = 0; i <= n; i++) {
            dp[i][1] = prevSum[i];
        }

        for(int m = 2; m <= k; m++) {
            for(int i = m; i <= n; i++) {
                for(int j = m - 1; j < i; j++) {
                    // j - i 段
                    int cur = prevSum[i] - prevSum[j];
                    int curMax = max(dp[j][m - 1], cur);
                    dp[i][m] = min(dp[i][m], curMax);
                }
            }
        }
        return dp[n][k];
    }
};
// @lc code=end

