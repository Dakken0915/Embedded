/*
 * @lc app=leetcode.cn id=3599 lang=cpp
 *
 * [3599] 划分数组得到最小 XOR
 */

// @lc code=start
class Solution {
public:
    int minXor(vector<int>& nums, int k) {
        int n = nums.size();

        // 求前缀异或
        vector<int> prev(n + 1, 0);
        for(int i = 1; i <= n; i++) {
            prev[i] = prev[i - 1] ^ nums[i - 1];
        }

        vector<vector<int>> dp(n + 1, vector<int>(k + 1, INT_MAX));
        for(int i = 0; i <= n; i++) {
            dp[i][1] = prev[i];
        }

        for(int m = 2; m <= k; m++) {
            // 分成 m 段至少需要 m 项
            for(int i = m; i <= n; i++) {
                for(int j = m - 1; j < i; j++) {
                    int seg_xor = prev[i] ^ prev[j];
                    // 取最大的最小值
                    int cur_max = max(dp[j][m-1], seg_xor);
                    dp[i][m] = min(dp[i][m], cur_max);
                }
            }
        }
        return dp[n][k];
    }
};
// @lc code=end

