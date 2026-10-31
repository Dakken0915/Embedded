/*
 * @lc app=leetcode.cn id=3742 lang=cpp
 *
 * [3742] 网格中得分最大的路径
 */

// @lc code=start
class Solution {
public:
    int maxPathScore(vector<vector<int>>& grid, int k) {
        int m = grid.size();
        int n = grid[0].size();
        // dp[i][j][c]：走到坐标 (i,j)，累计代价恰好为 c时，可以获得的最大分数
        vector<vector<vector<int>>> dp(m, vector<vector<int>>(n, vector<int>(k + 1, INT_MIN)));
        dp[0][0][k] = 0;
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if(grid[i][j] == 0) {
                    if(j != 0)
                        for(int q = 0; q <= k; q++)
                            dp[i][j][q] = dp[i][j - 1][q];
                    if(i != 0)
                        for(int q = 0; q <= k; q++)
                            dp[i][j][q] = max(dp[i][j][q], dp[i - 1][j][q]);
                }
                else if(grid[i][j] == 1) {
                    if(j != 0)
                        for(int q = 1; q <= k; q++)
                            dp[i][j][q - 1] = dp[i][j - 1][q] + 1;
                    if(i != 0)
                        for(int q = 1; q <= k; q++)
                            dp[i][j][q - 1] = max(dp[i][j][q - 1], dp[i - 1][j][q] + 1);
                }
                else {
                    if(j != 0)
                        for(int q = 1; q <= k; q++)
                            dp[i][j][q - 1] = dp[i][j - 1][q] + 2;
                    if(i != 0)
                        for(int q = 1; q <= k; q++)
                            dp[i][j][q - 1] = max(dp[i][j][q - 1], dp[i - 1][j][q] + 2);
                }
            }
        }
        int res = -1;
        for(int x : dp[m - 1][n - 1])
            res = max(res, x);
        return res;
    }
};
// @lc code=end

