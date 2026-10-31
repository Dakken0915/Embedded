/*
 * @lc app=leetcode.cn id=3144 lang=cpp
 *
 * [3144] 分割字符频率相等的最少子字符串
 */

// @lc code=start
class Solution {
public:
    int minimumSubstringsInPartition(string s) {
        int n = s.size();
        vector<int> dp(n + 1, INT_MAX);
        dp[0] = 0;

        for(int i = 1; i <= n; i++) {
            vector<int> cnt(26, 0);
            dp[i] = dp[i - 1] + 1;  // 最坏情况
            // 枚举前项
            for(int j = i - 1; j >= 0; j--) {
                cnt[s[j] - 'a']++;
                // 判断 s[j, i] 是否平衡
                bool ok = true;
                int temp = -1;
                for(int x : cnt) {
                    if(x == 0) continue;
                    if(temp == -1) temp = x;
                    if(x != temp)  // 出现不相等
                        ok = false;
                }
                if(ok)
                    dp[i] = min(dp[i], dp[j] + 1);
            }
        }
        return dp[n];
    }
};
// @lc code=end

