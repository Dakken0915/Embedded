/*
 * @lc app=leetcode.cn id=322 lang=cpp
 *
 * [322] 零钱兑换
 */

// @lc code=start
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        // 标记为 INT_MAX 的表示不可达 dp的索引表示要达到的金额
        vector<int> dp(amount + 1, INT_MAX);
        dp[0] = 0;   // 到达0元需要0个硬币
        
        // 先遍历每一种硬币（完全背包的标准写法）
        for(int coin : coins) {
            for(int i = coin; i < amount + 1; i++) {
                // 如果 i - coin 是可达的
                if(dp[i - coin] != INT_MAX)
                    dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
        return dp[amount] == INT_MAX ? -1 : dp[amount];
    }
};
// @lc code=end

class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        // 标记为 INT_MAX 的表示不可达 dp的索引表示要达到的金额
        vector<int> dp(amount + 1, INT_MAX);
        dp[0] = 0;   // 到达0元需要0个硬币

        for(int i = 1 ; i < amount + 1; i++) {
            for(int coin : coins) {
                if(i >= coin && dp[i - coin] != INT_MAX)
                    // dp[i] 保存到目前为止的最优解
                    dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
        return dp[amount] == INT_MAX ? -1 : dp[amount];
    }
};
