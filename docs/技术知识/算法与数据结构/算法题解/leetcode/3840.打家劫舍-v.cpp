/*
 * @lc app=leetcode.cn id=3840 lang=cpp
 *
 * [3840] 打家劫舍 V
 */

// @lc code=start
class Solution {
public:
    long long rob(vector<int>& nums, vector<int>& colors) {
        if(!nums.size()) return 0;
        if(nums.size() == 1) return nums[0];
        vector<long long> dp(nums.size(), 0);
        // 初始化
        dp[0] = nums[0];
        if(colors[0] == colors[1])
            dp[1] = max(nums[1], nums[0]);
        else
            dp[1] = dp[0] + nums[1];

        for(int i = 2; i < nums.size(); i++) {
            if(colors[i] == colors[i - 1])
                dp[i] = max(dp[i - 1], dp[i - 2] + nums[i]);
            else
                dp[i] = max(dp[i - 1] + nums[i], dp[i - 2] + nums[i]);
        }
        return dp[nums.size() - 1];
    }
};
// @lc code=end

