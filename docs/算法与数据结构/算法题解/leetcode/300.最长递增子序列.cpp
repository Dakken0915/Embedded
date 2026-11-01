/*
 * @lc app=leetcode.cn id=300 lang=cpp
 *
 * [300] 最长递增子序列
 */

// @lc code=start
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        // dfs -> dp数组  递归 -> 循环
        int n = nums.size();
        vector<int> dp(n, 0);

        for(int i = 0; i < n; i++) {
            for(int j = 0; j < i; j++) {
                if(nums[j] < nums[i])
                    dp[i] = max(dp[i], dp[j]);
            }
            dp[i] += 1;
        }
        return *max_element(dp.begin(), dp.end());
    }
};
// @lc code=end

// 递归思路解法
class Solution {
public:
    int dfs(vector<int>& nums, int index) {
        int res = 0;
        for(int i = 0; i < index; i++)
            // 枚举比自己小的前项，拆解成对于前项的子问题
            if(nums[i] < nums[index])
                res = max(res, dfs(nums, i));
        return res + 1;
    }

    int lengthOfLIS(vector<int>& nums) {
        int n = nums.size();
        int res = 0;
        for(int i = 0; i < n; i++)  // 找到以每一项为结尾的最大值
            res = max(res, dfs(nums, i));
        return res;
    }
};