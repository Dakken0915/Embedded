/*
 * @lc app=leetcode.cn id=704 lang=cpp
 *
 * [704] 二分查找
 */

// @lc code=start
class Solution {
public:
    int lower_bound(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;
        while(left <= right) {
            int mid = left + (right - left) / 2;
            if(nums[mid] >= target)
                right = mid - 1;
            else
                left = mid + 1;
        }
        return left;
    }

    int search(vector<int>& nums, int target) {
        int index = lower_bound(nums, target);
        if(index == nums.size() || nums[index] != target)
            return -1;
        return index;
    }
};
// @lc code=end

