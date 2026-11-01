    /*
    * @lc app=leetcode.cn id=583 lang=cpp
    *
    * [583] 两个字符串的删除操作
    */

    // @lc code=start
    class Solution {
    public:
        int minDistance(string word1, string word2) {
            // 相当于求最大公共子序列
            int n = word1.size();
            int m = word2.size();

            vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
            for(int i = 1; i <= n; i++) {
                for(int j = 1; j <= m; j++) {
                    if(word1[i - 1] == word2[j - 1])
                        dp[i][j] = dp[i - 1][j - 1] + 1;
                    else
                        dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);
                }
            }
            return n + m - 2 * dp[n][m];
        }
    };
    // @lc code=end

