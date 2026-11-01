/*
 * @lc app=leetcode.cn id=2200 lang=cpp
 *
 * [2200] 找出数组中的所有 K 近邻下标
 */

// @lc code=start
class Solution {
public:
    vector<int> findKDistantIndices(vector<int>& nums, int key, int k) {
        int left = 0; 
        int right = 0;
        vector<int> res;

        while(right <= nums.size() - 1) {
            if(nums[right] == key) {
                while(left <= right + k) {
                    // 记录符合条件的下标，left 单向移动，自动去重
                    if(left >= right - k && left < nums.size())
                        res.push_back(left);
                    left++;
                }
            }
            right++;
        }
        return res;
    }
};
// @lc code=end

