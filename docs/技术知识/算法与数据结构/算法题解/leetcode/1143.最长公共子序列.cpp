/*
 * @lc app=leetcode.cn id=1143 lang=cpp
 *
 * [1143] 最长公共子序列
 */

// @lc code=start
class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        int m = text1.size();
        int n = text2.size();

        // dp[i][j]：text1 的前 i 个字符，text2 的前 j 个字符的最长公共子序列长度
        vector dp(m + 1, vector<int>(n + 1, 0));
        for(int i = 1; i <= m; i++) {
            for(int j = 1; j <= n; j++) {
                dp[i][j] = text1[i - 1] == text2[j - 1] ? dp[i - 1][j - 1] + 1 :\
                    max(dp[i][j - 1], dp[i - 1][j]);
            }
        }
        return dp[m][n];
    }
};
// @lc code=end

// 递归解法

class Solution {
private:
    vector<vector<int>> memo;
    int dfs(string& text1, string& text2, int i, int j) {
        if(i < 0 || j < 0) return 0;

        int& res = memo[i][j]; // 注意这里是引用
        if (res != -1)
            return res; // 之前计算过

        // 两个都选
        if(text1[i] == text2[j]) 
            return res = dfs(text1, text2, i - 1, j - 1) + 1;
        // 选其中一个
        return res = max(dfs(text1, text2, i, j - 1), dfs(text1, text2, i - 1, j));
    }

public:
    int longestCommonSubsequence(string text1, string text2) {
        // 拆分成选与不选的子问题，确定递归条件
        int n = text1.size();
        int m = text2.size();
        memo.assign(n, vector<int>(m, -1)); // -1表示未被计算过
        return dfs(text1, text2, n - 1, m - 1);
    }
};
