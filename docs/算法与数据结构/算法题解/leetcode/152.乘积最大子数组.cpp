/*
 * @lc app=leetcode.cn id=152 lang=cpp
 *
 * [152] 乘积最大子数组
 */

// @lc code=start
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        if(nums.size() == 1) return nums[0];
        vector<int> dp(nums.size(), 0);

        // 负数 × 负数会变正数 同时保存最大值和最小值
        int curMin = nums[0];
        int curMax = nums[0];
        int ans = nums[0];
        for(int i = 1; i < nums.size(); i++) {
            int tempMax = max(max(nums[i], nums[i] * curMax), nums[i] * curMin);
            int tempMin = min(min(nums[i], nums[i] * curMax), nums[i] * curMin);
            curMax = tempMax;
            curMin = tempMin;
            ans = max(ans, tempMax);
        }
        return ans;
    }
};
// @lc code=end

