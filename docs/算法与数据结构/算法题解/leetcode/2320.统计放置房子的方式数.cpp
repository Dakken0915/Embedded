/*
 * @lc app=leetcode.cn id=2320 lang=cpp
 *
 * [2320] 统计放置房子的方式数
 */

// @lc code=start
class Solution {
public:
    int MOD = 1e9 + 7;

    int rob(int n) {
        int f0 = 1;
        int f1 = 2;
        for(int i = 2; i <= n; i++) {
            // 不放房子：i-1 块随便摆 → dp[i-1]
            // 放房子：第 i-1 块不能放，i-2 块随便摆 → dp[i-2]
            int cur = (f0 + f1) % MOD;
            f0 = f1;
            f1 = cur;
        }
        return f1;
    }
    int countHousePlacements(int n) {
        // 1LL 先防止溢出
        return 1LL * rob(n) * rob(n) % MOD;
    }
};
// @lc code=end

