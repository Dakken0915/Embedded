/*
 * @lc app=leetcode.cn id=34 lang=cpp
 *
 * [34] 在排序数组中查找元素的第一个和最后一个位置
 */

// @lc code=start
class Solution {
public:
    // lower_bound 返回最小的满足 nums[i] >= target 的下标 i
    // 如果数组为空，或者所有数都 < target，则返回 nums.size()
    // 要求 nums 是非递减的，即 nums[i] <= nums[i + 1]
    int lower_bound(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;

        while(left <= right) { // 区间不为空
            int mid = left + (right - left) / 2;
            if(nums[mid] >= target)
                right = mid - 1; // 范围缩小到 [left, mid-1]
            else
                left = mid + 1;  // 范围缩小到 [mid+1, right]
        }
        return left;
    }

    vector<int> searchRange(vector<int>& nums, int target) {
        int start = lower_bound(nums, target);
        if(start == nums.size() || nums[start] != target)
            return {-1, -1};
        // 如果 start 存在，那么 end 必定存在
        int end = lower_bound(nums, target + 1) - 1;
        return {start, end};
    }
};
// @lc code=end

