/*
 * @lc app=leetcode.cn id=27 lang=cpp
 *
 * [27] 移除元素
 */

// @lc code=start
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int n = nums.size();
        int left = 0;
        int right = n - 1;
        int res = 0;  // 计数
        while(left <= right) {
            if(nums[left] == val) {
                swap(nums[left], nums[right--]);
                res++;
            }
            else left++;
        }
        return n - res;
    }
};
// @lc code=end

