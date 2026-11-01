/*
 * @lc app=leetcode.cn id=1793 lang=cpp
 *
 * [1793] 好子数组的最大分数
 */

// @lc code=start
class Solution {
public:
    int maximumScore(vector<int>& nums, int k) {
        int n = nums.size();
        int left = k;
        int right = k;
        int res = nums[k];
        int minL = nums[k];
        int minR = nums[k];
        while(left > 0 || right < n - 1) {
            if(left == 0) right++;
            else if(right == n - 1) left--;
            // 贪心策略：谁大谁移动
            else if(nums[left-1] > nums[right+1]) left--;
            else right++;

            if(left >= 0) minL = min(minL, nums[left]);
            if(right < n) minR = min(minR, nums[right]);

            res = max(res, min(minL, minR) * (right-left+1));
        }
        return res;
    }
};
// @lc code=end
