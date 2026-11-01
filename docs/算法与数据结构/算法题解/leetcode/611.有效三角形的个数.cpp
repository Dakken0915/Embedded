/*
 * @lc app=leetcode.cn id=611 lang=cpp
 *
 * [611] 有效三角形的个数
 */

// @lc code=start
class Solution {
public:
    int triangleNumber(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int n = nums.size();
        
        int res = 0;
        for(int i = n - 1; i >= 2; i--) {
            int left = 0;
            int right = i - 1;
            while(left < right) {
                if(nums[left] + nums[right] > nums[i]) {
                    res += right - left;
                    right--;
                }
                else left++;
            }
        }
        return res;
    }
};
// @lc code=end

