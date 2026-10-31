/*
 * @lc app=leetcode.cn id=1411 lang=cpp
 *
 * [1411] 给 N x 3 网格图涂色的方案数
 */

// @lc code=start
class Solution {
public:
    int numOfWays(int n) {
        const int MOD = 1e9 + 7;
        vector<int> f(n + 1);
        f[0] = 3;
        f[1] = 12;
        for (int i = 2; i <= n; i++) {
            f[i] = (f[i - 1] * 5LL - f[i - 2] * 2LL) % MOD; // 注意这里有减法，结果可能是负数
        }
        return (f[n] + MOD) % MOD; // 保证结果非负
    }
};
// @lc code=end

