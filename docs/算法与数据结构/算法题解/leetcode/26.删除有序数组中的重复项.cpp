/*
 * @lc app=leetcode.cn id=26 lang=cpp
 *
 * [26] 删除有序数组中的重复项
 */

// @lc code=start

// 快慢指针标准解法
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if(nums.empty()) return 0;
        int slow = 1;
        for(int fast = 1; fast < nums.size(); fast++) {
            // 快指针每找到一个新值就赋给慢指针
            if(nums[fast] != nums[slow - 1]){
                nums[slow++] = nums[fast];
            }
        }
        return slow;
    }
};
// @lc code=end

class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int n = nums.size();
        int cur = nums[0];
        for(int i = 1; i < n; i++) {
            while(i < n && nums[i] == nums[i - 1]) {
                nums.erase(nums.begin() + i);
                n--;
            }
            cur = nums[i];
        }
        return n;
    }
};
