/*
 * @lc app=leetcode.cn id=2606 lang=cpp
 *
 * [2606] 找到最大开销的子字符串
 */

// @lc code=start
class Solution {
public:
    int maximumCostSubstring(string s, string chars, vector<int>& vals) {
        if(s.empty()) return 0;
        int maxVal = 0;
        unordered_map<char, int> mp;
        for(int i = 0; i < chars.size(); i++) {
            mp[chars[i]] = vals[i];
        }

        vector<int> dp(s.size(), 0);

        if(mp.find(s[0]) != mp.end()) dp[0] = mp[s[0]];
        else dp[0] = s[0] - 'a' + 1;
        maxVal = max(maxVal, dp[0]);

        for(int i = 1; i < s.size(); i++) {
            // 要么取自身数值并截断，要么取子串
            int val = 0;
            if(mp.find(s[i]) != mp.end()) val = mp[s[i]];
            else val = s[i] - 'a' + 1;
            dp[i] = max(val, dp[i - 1] + val);
            maxVal = max(maxVal, dp[i]);
        }
        return maxVal;
    }
};
// @lc code=end

