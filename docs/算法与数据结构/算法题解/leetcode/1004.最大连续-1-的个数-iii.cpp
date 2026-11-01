/*
 * @lc app=leetcode.cn id=1004 lang=cpp
 *
 * [1004] 最大连续1的个数 III
 */

// @lc code=start
class Solution {
public:
    int longestOnes(vector<int>& nums, int k) {
        int n = nums.size();
        int res = INT_MIN;

        int cnt = 0; // 用来记录窗口内 0 的个数
        int left = 0;
        for(int i = 0; i < n; i++) {
            if(nums[i] == 0)
                cnt++;
            
            // 不满足条件，缩短窗口
            while(cnt > k) {
                if(nums[left] == 0)
                    cnt--;
                left++;
            }
            // 满足条件时统计
            res = max(res, i - left + 1);
        }
        return res;
    }
};
// @lc code=end

