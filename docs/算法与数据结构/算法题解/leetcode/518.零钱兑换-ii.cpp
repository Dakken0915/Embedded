/*
 * @lc app=leetcode.cn id=518 lang=cpp
 *
 * [518] 零钱兑换 II
 */

// @lc code=start
// 完全背包解法
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        // unsigned自动对 2^N 取模
        vector<unsigned> dp(amount + 1, 0);
        dp[0] = 1; //基准：凑0元1种方案
        for(int coin : coins) {
            //完全背包 正序遍历容量
            for(int i = coin; i <= amount; i++) {
                // 原先凑出 j 的所有方案全部保留，也就是 `dp[j]` 本身
                // 决定选这一枚硬币：那剩下需要凑的金额是 `j - c`
                dp[i] += dp[i - coin];
            }
        }
        return dp[amount];
    }
};
// @lc code=end

// 0-1背包求最大容量
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        int n = coins.size();
        vector dp(amount + 1, 0);
        for(int i = 0; i < n; i++) {
            int w = coins[i];
            //01背包：容量逆序遍历！全部j都要更新
            for(int j = amount; j >= w; j--){
                dp[j] = max(dp[j], dp[j - w] + 1);
            }
        }
        return dp[amount];
    }
};