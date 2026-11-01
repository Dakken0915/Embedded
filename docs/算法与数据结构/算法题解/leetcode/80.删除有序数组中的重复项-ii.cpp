/*
 * @lc app=leetcode.cn id=80 lang=cpp
 *
 * [80] 删除有序数组中的重复项 II
 */

// @lc code=start
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int n = nums.size();
        if(n == 0) return 0;
        int cnt = 0; 
        int left = 0;
        int right = 1;
        while(right < n) {
            if(nums[right] == nums[left]) {
                cnt++;
                if (cnt < 2) { // 重复一次
                    left++;
                    nums[left] = nums[right];
                }
                right++;
            }
            else { // 新元素
                cnt = 0;
                left++;
                nums[left] = nums[right];
                right++;
            }
        }
        return left + 1;
    }
};
// @lc code=end

