/*
 * @lc app=leetcode.cn id=3376 lang=cpp
 *
 * [3376] 破解锁的最少时间 I
 */

// @lc code=start
class Solution {
public:
    // 状态压缩DP
    int findMinimumTime(vector<int>& strength, int k) {
        int n = strength.size();
        int m = 1 << n;
        vector<int> dp(m, INT_MAX);
        dp[0] = 0;
        // 掩码 1 代表开锁了
        for(int i = 0; i < m; i++) {
            // 计算每分钟增量
            int x = 1 + k * (popcount((unsigned)i) - 1);
            for(int j = 0; j < n; j++) {
                if(i >> j & 1)
                    dp[i] = min(dp[i], dp[i ^ (1 << j)] + (strength[j] + x - 1) / x);
            }
        }
        return dp[m - 1];
    }
};
// @lc code=end

