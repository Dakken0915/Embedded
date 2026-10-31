/*
 * @lc app=leetcode.cn id=2466 lang=cpp
 *
 * [2466] 统计构造好字符串的方案数
 */

// @lc code=start
class Solution {
public:
    int countGoodStrings(int low, int high, int zero, int one) {
        // 要么加 0，要么加 1
        int MOD = 1e9 + 7;
        vector<int> dp(high + 1, 0);
        dp[0] = 1;  // 空字符串为一种
        for(int i = 1; i <= high; i++) {
            long long sum = 0;
            if(i >= zero)
                sum += dp[i - zero];
            if(i >= one)
                sum += dp[i - one];
            dp[i] = sum % MOD;
        }
        int res = 0;
        for(int i = low; i <= high; i++)
            res = (res + dp[i]) % MOD;
        return res;
    }
};
// @lc code=end

