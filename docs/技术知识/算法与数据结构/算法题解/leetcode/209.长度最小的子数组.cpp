/*
 * @lc app=leetcode.cn id=209 lang=cpp
 *
 * [209] 长度最小的子数组
 */

// @lc code=start
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int n = nums.size();
        int res = INT_MAX;

        int left = 0;
        int sum = 0;
        for(int i = 0; i < n; i++) {
            sum += nums[i];
            // 不符合最小要求
            while(sum - nums[left] >= target) {
                sum -= nums[left];
                left++;
            }
            if(sum >= target)
                res = min(res, i - left + 1);
        }
        return res == INT_MAX ? 0 : res;
    }
};
// @lc code=end

