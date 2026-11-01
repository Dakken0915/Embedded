/*
 * @lc app=leetcode.cn id=2707 lang=cpp
 *
 * [2707] 字符串中的额外字符
 */

// @lc code=start
class Solution {
public:
    int minExtraChar(string s, vector<string>& dictionary) {
        unordered_set<string> set;
        int maxLen = 0;
        for(string str : dictionary) {
            maxLen = max(maxLen, (int)str.size());
            set.insert(str);
        }
        int n = s.size();

        // 字符串 s 的前 i 个字符 s [0 … i‑1]`，最少有多少个额外字符
        vector<int> dp(n + 1, 0);
        for(int i = 1; i <= n; i++) {

            int res = dp[i - 1] + 1; // 直接当作额外字符

            for(int j = i - 1; j >= max(i - maxLen, 0); j--) {
                if(set.find(s.substr(j, i - j)) != set.end())
                    res = min(res, dp[j]);
            }
            dp[i] = res;
        }
        return dp[n];
    }
};
// @lc code=end

