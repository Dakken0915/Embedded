/*
 * @lc app=leetcode.cn id=2266 lang=cpp
 *
 * [2266] 统计打字方案数
 */

// @lc code=start
class Solution {
public:
    int MOD = 1e9 + 7;

    long long process(int len, char s) {
        vector<long long> dp(len + 1, 0);
        dp[0] = 1;
        if(s == '7' || s == '9') {
            for(int i = 1; i <= len; i++)
                for(int j = i - 1; j >= max(i - 4, 0); j--)  // 爬3层模型
                    dp[i] = (dp[j] + dp[i]) % MOD;
            return dp[len];
        }
        else {
            for(int i = 1; i <= len; i++)
                for(int j = i - 1; j >= max(i - 3, 0); j--)  // 爬3层模型
                    dp[i] = (dp[j] + dp[i]) % MOD;
            return dp[len];
        }  
    }

    // 本质是爬楼梯的方案
    int countTexts(string pressedKeys) {
        int n = pressedKeys.size();

        int res = 1;   // 计算每一组的乘积

        // 划分字符串, 要同时存字符，7和9 是4层爬楼梯模型
        vector<pair<char, int>> count;
        int i = 0;
        int j = 0;
        while(i < n) {
            while(j < n && pressedKeys[i] == pressedKeys[j])
                j++;
            int len = j - i;
            count.push_back({pressedKeys[i], len});
            i = j;
        }

        for(int k = 0; k < count.size(); k++) {
            res = res * process(count[k].second, count[k].first) % MOD;
        }
        return res;
    }
};
// @lc code=end
