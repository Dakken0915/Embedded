/*
 * @lc app=leetcode.cn id=75 lang=cpp
 *
 * [75] 颜色分类
 */

// @lc code=start
class Solution {
private:
    void partition(vector<int>& nums, int L, int R) {
        int less = L - 1; // 小于区域右边界
        int more = R + 1; // 大于区域左边界
        while(L < more) {
            if(nums[L] < 1)
                swap(nums[L++], nums[++less]);   // 延伸区域
            else if(nums[L] == 1)
                L++;
            else
                swap(nums[L], nums[--more]);
        }
    }

public:
    void sortColors(vector<int>& nums) {
        // 三路快排
        partition(nums, 0, nums.size() - 1);
    }
};
// @lc code=end

