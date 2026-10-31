/*
 * @lc app=leetcode.cn id=132 lang=cpp
 *
 * [132] 分割回文串 II
 */

// @lc code=start
class Solution {
public:
    int minCut(string s) {
        int n = s.size();
        // ispal[l][r] 表示 s[l] 到 s[r] 是否为回文串
        vector<vector<bool>> ispal(n, vector<bool>(n, true));
        for(int l = n - 2; l >= 0; l--) {
            for(int r = l + 1; r < n; r++) {
                if(s[l] == s[r] && ispal[l + 1][r - 1])
                    ispal[l][r] = true;
                else
                    ispal[l][r] = false;
            }
        }
        vector<int> dp(n);
        for(int i = 0; i < n; i++) {
            // 已经是回文串，无需判定
            if(ispal[0][i]) continue;

            int res = INT_MAX;
            // 枚举前面的划分找到最小划分次数
            for(int j = i; j > 0; j--) {
                if(ispal[j][i])
                    res = min(res, dp[j - 1] + 1);
            }
            dp[i] = res;
        }
        return dp[n - 1];
    }
};
// @lc code=end

// 示例
class Solution {
public:
    int minCut(string s) {
        int n = s.size();
        // is_palindrome[l][r] 表示 s[l] 到 s[r] 是否为回文串
        vector is_palindrome(n, vector<bool>(n, true));
        for (int l = n - 2; l >= 0; l--) {
            for (int r = l + 1; r < n; r++) {
                is_palindrome[l][r] = s[l] == s[r] && is_palindrome[l + 1][r - 1];
            }
        }

        vector<int> f(n);
        for (int r = 0; r < n; r++) {
            if (is_palindrome[0][r]) { // 已是回文串，无需分割
                continue;
            }
            int res = INT_MAX;
            for (int l = 1; l <= r; l++) { // 枚举分割位置
                if (is_palindrome[l][r]) {
                    res = min(res, f[l - 1] + 1); // 在 l-1 和 l 之间切一刀
                }
            }
            f[r] = res;
        }
        return f[n - 1];
    }
};