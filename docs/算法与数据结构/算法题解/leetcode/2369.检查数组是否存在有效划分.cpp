/*
 * @lc app=leetcode.cn id=2369 lang=cpp
 *
 * [2369] 检查数组是否存在有效划分
 */

// @lc code=start
class Solution {
public:
    bool validPartition(vector<int>& nums) {
        int n = nums.size();
        vector<bool> f(n + 1, false);
        f[0] = true;
        for(int i = 1; i < n; i++) {
            if(f[i - 1] && nums[i] == nums[i - 1] 
                || i > 1 && f[i - 2] && (nums[i] == nums[i - 1] && nums[i] == nums[i - 2] 
                || nums[i] == nums[i - 1] + 1 && nums[i] == nums[i - 2] + 2))
                f[i + 1] = true;
        }
        return f[n];
    }
};
// @lc code=end

