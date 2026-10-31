/*
 * @lc app=leetcode.cn id=879 lang=cpp
 *
 * [879] 盈利计划
 */

// @lc code=start
class Solution {
public:
    int profitableSchemes(int n, int minProfit, vector<int>& group, vector<int>& profit) {
        const int MOD = 1e9 + 7;
        int m = group.size();

        // dp[i][j] 表示在 i 名员工和获得 j 的利润下的方式
        vector<vector<int>> dp(n + 1, vector<int>(minProfit + 1, 0));
        dp[0][0] = 1;
        for(int i = 0; i < m; i++) {  // item
            int workers = group[i];
            int earn = profit[i];
            vector<vector<int>> ndp = dp; // 不选当前项目
            for(int j = 0; j <= n; j++) {
                for(int k = 0; k <= minProfit; k++) {
                    if(dp[j][k] == 0) continue;

                    int nj = j + workers;
                    if(nj > n) continue;
                    int nk = min(k + earn, minProfit);
                    // 选当前项目
                    ndp[nj][nk] = (ndp[nj][nk] + dp[j][k]) % MOD;
                }
            }
            dp.swap(ndp);
        }
        int res = 0;
        for(int j = 0; j <= n; j++)
            // 累加所有符合条件的答案
            res = (res + dp[j][minProfit]) % MOD;

        return res;
    }
};
// @lc code=end

