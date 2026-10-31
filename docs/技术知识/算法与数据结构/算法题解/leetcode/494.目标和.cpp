/*
 * @lc app=leetcode.cn id=494 lang=cpp
 *
 * [494] 目标和
 */

// @lc code=start

// 标准0-1背包DP
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        int sum = reduce(nums.begin(), nums.end());
        if((sum - abs(target)) % 2 != 0) return 0;
        int p = (sum - target) / 2;
        if(p < 0) return 0;
        int n = nums.size();

        // 在已经遍历过的数字中，选出若干数字，总和恰好等于 `j` 的方案总数。
        vector<vector<int>> dp(n+1, vector<int>(p+1, 0));
        dp[0][0] = 1; // 0个数凑0，1种方案

        for(int i = 1; i <= n; i++){   // 枚举物品
            int x = nums[i-1];
            for(int j = 0; j <= p; j++){  // 枚举容量
                dp[i][j] = dp[i-1][j];    // 不选
                if(j >= x) dp[i][j] += dp[i-1][j-x]; // 可选
            }
        }
        return dp[n][p];
    }
};
// @lc code=end

// 0-1背包递归解法
class Solution {
private:
    // 第 i 个元素，c: 目标容量
    int dfs(vector<int>& nums, int i, int c) {
        if(i < 0) return c == 0 ? 1 : 0;

        // 放不下，不选当前元素
        if(c < nums[i]) return dfs(nums, i - 1, c);

        // 不选 + 选
        return dfs(nums, i - 1, c) + dfs(nums, i - 1, c - nums[i]);
    }
public:
    // p = sum - target / 2
    // 选一些数和为 p
    int findTargetSumWays(vector<int>& nums, int target) {
        int sum = reduce(nums.begin(), nums.end());
        if((sum - abs(target)) % 2 != 0) return 0;
        int p = (sum - target) / 2;  // 背包容量

        int n = nums.size();

        return dfs(nums, n - 1, p);
    }
};