/*
 * @lc app=leetcode.cn id=3418 lang=cpp
 *
 * [3418] 机器人可以获得的最大金币数
 */

// @lc code=start
class Solution {
public:
    int maximumAmount(vector<vector<int>>& coins) {
        int m = coins.size();
        int n = coins[0].size();

        // dp[m][n][k] k表示剩余的感化次数
        vector<vector<vector<long long>>> dp(m, vector<vector<long long>>(n, vector<long long>(3, INT_MIN)));
        dp[0][0][2] = coins[0][0];
        if(coins[0][0] < 0)
            dp[0][0][1] = 0;

        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if(coins[i][j] >= 0) {
                    if(i > 0)
                        for(int x = 0; x < 3; x++)
                            dp[i][j][x] = dp[i-1][j][x] + coins[i][j];
                    if(j > 0)
                        for(int x = 0; x < 3; x++)
                            dp[i][j][x] = max(dp[i][j][x], dp[i][j-1][x] + coins[i][j]);
                }
                else {
                    if(i > 0) { // 选择感化或不感化
                        for(int x = 0; x < 3; x++)  // 不感化
                            dp[i][j][x] = dp[i-1][j][x] + coins[i][j];
                        for(int x = 0; x < 2; x++)  // 感化
                            dp[i][j][x] = max(dp[i][j][x], dp[i-1][j][x + 1]);
                    }
                    if(j > 0) {
                        for(int x = 0; x < 3; x++)  // 不感化
                            dp[i][j][x] = max(dp[i][j][x], dp[i][j-1][x] + coins[i][j]);
                        for(int x = 0; x < 2; x++)  // 感化
                            dp[i][j][x] = max(dp[i][j][x], dp[i][j-1][x + 1]);
                    }
                }
            }
        }
        int res = INT_MIN;  // 结果有可能是负数
        for(int coin : dp[m - 1][n - 1])
            res = max(res, coin);

        return res;
    }
};
// @lc code=end

