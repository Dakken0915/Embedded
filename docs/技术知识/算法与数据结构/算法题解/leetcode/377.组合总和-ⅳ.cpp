/*
 * @lc app=leetcode.cn id=377 lang=cpp
 *
 * [377] 组合总和 Ⅳ
 */

// @lc code=start
// 完全背包
class Solution {
public:
    int combinationSum4(vector<int>& nums, int target) {
        int n = nums.size();
        // dp[i] 表示凑出总和 = i 的方案排列总数
        vector<unsigned> dp(target + 1, 0);
        dp[0] = 1;
        for(int i = 1; i <= target; i++) {  // 枚举容量
            for(int num : nums) {       // 枚举物品
                if(num <= i)
                    dp[i] += dp[i - num];
            }
        }
        return dp[target];
    }
};
// @lc code=end

