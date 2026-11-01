/*
 * @lc app=leetcode.cn id=2787 lang=cpp
 *
 * [2787] 将一个数字表示成幂的和的方案数
 */

// @lc code=start
class Solution {
public:
    const int MOD = 1e9 + 7;
    int pow(int num, int x) {
        int res = 1;
        while(x--)
            res *= num;
        return res;
    }

    int numberOfWays(int n, int x) {
        int temp = 1;
        while(pow(temp, x) <= n)
            temp++;
        int maxNum = temp - 1;

        vector<int> dp(n + 1, 0);
        dp[0] = 1;
        for(int i = 1; i <= maxNum; i++) { // 枚举项目
            long long val = pow(i, x);
            for(int j = n; j >= val; j--) { // 枚举容量
                 // dp[j]：不选i； dp[j-val]：选i
                dp[j] = (dp[j] + dp[j - val]) % MOD;
            }
        }
        return dp[n];
    }
};
// @lc code=end

