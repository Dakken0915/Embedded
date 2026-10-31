/*
 * @lc app=leetcode.cn id=3361 lang=cpp
 *
 * [3361] 两个字符串的切换距离
 */

// @lc code=start
class Solution {
public:
    long long shiftDistance(string s, string t, vector<int>& nextCost, vector<int>& previousCost) {
        // nextCost[i]: 从字母 'a' 顺时针移动到 i 的代价
        vector<long long> next(27, 0);
        // prev[i]: 从第 i 个字母逆时针到 'a' 的累计代价
        vector<long long> prev(26, 0);
        for(int i = 0;i < 26; i++)
            next[i+1] = next[i] + nextCost[i];
        
        for(int k = 1; k < 26; k++)
            prev[k] = prev[k-1] + previousCost[k];

        long long sumN = next[26];
        long long sumP = prev[25] + previousCost[0];
        long long res = 0;
        for(int i = 0; i < s.size(); i++) {
            int u = s[i]-'a';
            int v = t[i]-'a';
            if(u == v) continue;

            long long forward;
            if(v >= u) forward = next[v] - next[u];
            else forward = sumN - (next[u] - next[v]);

            long long backward;
            if(v <= u) backward = prev[u] - prev[v];
            else backward = sumP - (prev[v] - prev[u]);

            res += min(forward, backward);
        }
        return res;
    }
};
// @lc code=end

