/*
 * @lc app=leetcode.cn id=2540 lang=cpp
 *
 * [2540] 最小公共值
 */

// @lc code=start
class Solution {
public:
    int getCommon(vector<int>& nums1, vector<int>& nums2) {
        int i = 0;
        int j = 0;
        while(i < nums1.size() && j < nums2.size()) {
            if(nums1[i] == nums2[j]) return nums1[i];
            // 谁小谁移动
            else if(nums1[i] <= nums2[j]) i++;
            else j++;
        }
        return -1;
    }
};
// @lc code=end

