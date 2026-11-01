/*
 * @lc app=leetcode.cn id=53 lang=cpp
 *
 * [53] 最大子数组和
 */

// @lc code=start
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        vector<int> dp(nums.size(), 0);

        dp[0] = nums[0];
        int res = nums[0];
        for(int i = 1; i < nums.size(); i++) {
            // 要么只取当前数字，要么取之前子数组 + 当前数字
            dp[i] = max(nums[i], dp[i - 1] + nums[i]);
            res = max(res, dp[i]);
        }
        return res;
    }
};
// @lc code=end

