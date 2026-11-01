/*
 * @lc app=leetcode.cn id=3914 lang=cpp
 *
 * [3914] 使数组非递减需要的最小累计值
 */

// @lc code=start
class Solution {
public:
    long long minOperations(vector<int>& nums) {
        long long ans = 0;
        for(int i = 1; i < nums.size(); i++)
            ans += max(nums[i - 1] - nums[i], 0);
        return ans;
    }
};
// @lc code=end

