/*
 * @lc app=leetcode.cn id=2779 lang=cpp
 *
 * [2779] 数组的最大美丽值
 */

// @lc code=start
class Solution {
public:
    int maximumBeauty(vector<int>& nums, int k) {
        if(nums.empty()) return 0;
        int n = nums.size();
        int res = 1;   // 答案至少为 1

        // 转换成符合 nums[i] - nums[left] <= 2k 的最大数量
        sort(nums.begin(), nums.end());
        int left = 0;
        for(int i = 0; i < n; i++) {
            // 不符合条件时收缩窗口
            while(nums[i] - nums[left] > 2 * k)
                left++;

            // 符合条件时更新答案
            res = max(res, i - left + 1);
        }
        return res;
    }
};
// @lc code=end

