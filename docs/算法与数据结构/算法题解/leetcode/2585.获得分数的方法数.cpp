/*
 * @lc app=leetcode.cn id=2585 lang=cpp
 *
 * [2585] 获得分数的方法数
 */

// @lc code=start
class Solution {
public:
    int waysToReachTarget(int target, vector<vector<int>>& types) {
        const int MOD = 1e9 + 7;
        int n = types.size();

        vector<int> dp(target + 1, 0);
        dp[0] = 1;   // 获取0分有一种方式

        for(int i = 0; i < n; i++) {
            int cnt = types[i][0];  // 题数
            int score = types[i][1];  // 单题得分
            for(int j = target; j >= score; j--) {
                // k：本组选k道题，k∈ [1,cnt]
                for(int k = 1; k <= cnt; k++) {
                    if(j - k * score < 0) continue;
                    dp[j] = (dp[j] + dp[j - score * k]) % MOD;
                }
            }
        }
        return dp[target];
    }
};
// @lc code=end

class Solution {
public:
    int waysToReachTarget(int target, vector<vector<int>>& types) {
        const int MOD = 1e9 + 7;
        int n = types.size();

        vector<int> dp(target + 1, 0);
        dp[0] = 1;   // 获取0分有一种方式

        // 多重背包展开成 01背包
        for(int i = 0; i < n; i++) {
            int cnt = types[i][0];  // 题数
            int score = types[i][1];  // 单题得分
            vector<int> ndp = dp;
            // 要去掉 k = 0 的方案
            for(int k = 1; k <= cnt; k++) {
                for(int j = target; j >= 0; j--) {
                    int total = k * score;
                    if(j - total < 0) break;
                    ndp[j] = (ndp[j] + dp[j - total]) % MOD;
                }
            }
            dp.swap(ndp);
        }
        return dp[target];
    }
};