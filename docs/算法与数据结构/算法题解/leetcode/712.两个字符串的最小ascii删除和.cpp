/*
 * @lc app=leetcode.cn id=712 lang=cpp
 *
 * [712] 两个字符串的最小ASCII删除和
 */

// @lc code=start
class Solution {
public:
    int minimumDeleteSum(string s1, string s2) {
        // a - 97
        int n = s1.size();
        int m = s2.size();

        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

        // 求最大公共子序列 ASCLL码值
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= m; j++) {
                if(s1[i - 1] == s2[j - 1])
                    dp[i][j] = dp[i - 1][j - 1] + 2 * (int)s1[i - 1];
                else
                    dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
            }
        }
        // 求 ASCLL码值的总和
        int sum = 0;
        for(char ch : s1)
            sum += (int)ch;
        for(char ch : s2)
            sum += (int)ch;
        return sum - dp[n][m];
    }
};
// @lc code=end

