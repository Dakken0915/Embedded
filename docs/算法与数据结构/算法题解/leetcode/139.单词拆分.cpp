    /*
    * @lc app=leetcode.cn id=139 lang=cpp
    *
    * [139] 单词拆分
    */

    // @lc code=start
    class Solution {
    public:
        bool wordBreak(string s, vector<string>& wordDict) {
            int maxLen = 0;
            unordered_set<string> set;
            for(string& str : wordDict) {
                maxLen = max(maxLen, (int)str.size());
                set.insert(str);
            }

            int n = s.size();
            // dp[i] 前 i项合不合法
            vector<bool> dp(n + 1, false);
            dp[0] = true;
            for(int i = 0; i <= n; i++) {
                // 枚举长度
                for(int j = i - 1; j >= max(i - maxLen, 0); j--) {
                    int len = i - j;
                    if(dp[j] && set.find(s.substr(j, len)) != set.end())
                        dp[i] = true;
                }
            }
            return dp[n];
        }
    };
    // @lc code=end

