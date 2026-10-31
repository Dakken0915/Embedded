/*
 * @lc app=leetcode.cn id=526 lang=cpp
 *
 * [526] 优美的排列
 */

// @lc code=start
class Solution {
public:
    int countArrangement(int n) {
        vector<int> dp(1 << n);
        dp[0] = 1;  // 一个都没选
        // 枚举每种情况
        for(int s = 1; s < 1 << n; s++) {
            // 统计选到第几位
            int i = popcount((unsigned)s);
            // 尝试选每一位
            for(int j = 1; j <= n; j++) {
                if(s >> (j - 1) & 1 && (i % j == 0 || j % i == 0))
                    dp[s] += dp[s ^ (1 << (j - 1))];
            }
        }
        return dp[(1 << n) - 1];
    }
};
// @lc code=end

