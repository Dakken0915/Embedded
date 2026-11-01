/*
 * @lc app=leetcode.cn id=741 lang=cpp
 *
 * [741] 摘樱桃
 */

// @lc code=start
class Solution {
public:
    // second
    int cherryPickup(vector<vector<int>>& grid) {
        int n = grid.size();
        // dp[step][c1][c2]，c1,c2 ∈ [0,n‑1]
        vector<vector<vector<int>>> dp(2*n-1, vector<vector<int>>(n, vector<int>(n, INT_MIN)));
        dp[0][0][0] = grid[0][0];

        for(int step = 1; step < 2*n-1; ++step)
        {
            for(int c1 = 0; c1 < n; c1++)
            {
                int r1 = step - c1;
                if(r1 < 0 || r1 >= n) continue;
                if(grid[r1][c1] == -1) continue; // 荆棘，此位置不可达

                for(int c2 = 0; c2 < n; c2++)
                {
                    int r2 = step - c2;
                    if(r2 < 0 || r2 >= n) continue;
                    if(grid[r2][c2] == -1) continue;

                    // 取4个前驱的最大值，要判断下标>=0
                    int pre = INT_MIN;
                    if(c1 >= 0 && c2 >=0) pre = max(pre, dp[step-1][c1][c2]);
                    if(c1-1 >=0 && c2 >=0) pre = max(pre, dp[step-1][c1-1][c2]);
                    if(c1 >=0 && c2-1 >=0) pre = max(pre, dp[step-1][c1][c2-1]);
                    if(c1-1 >=0 && c2-1 >=0) pre = max(pre, dp[step-1][c1-1][c2-1]);

                    if(pre == INT_MIN) continue; // 没有合法前驱，保持INT_MIN

                    int add = grid[r1][c1];
                    if(c1 != c2) add += grid[r2][c2];
                    dp[step][c1][c2] = pre + add;
                }
            }
        }
        return max(dp[2*n-2][n-1][n-1], 0);
    }
};
// @lc code=end

class Solution {
public:
    int cherryPickup(vector<vector<int>>& grid) {
        // 相当于有两条从 (0,0) 到 (n−1,n−1) 的路径
        int n = grid.size();
        // 创建三维DP数组
        // 第一维：步数t，范围 0 到 2n-2，共 2n-1 个
        // 第二维：A的横坐标j，范围 0 到 n，用 j+1 索引
        // 第三维：B的横坐标k，范围 0 到 n，用 k+1 索引
        // 初始化为 INT_MIN 表示不可达状态
        vector f(n * 2 - 1, vector(n + 1, vector<int>(n + 1, INT_MIN)));
        // 在步数 t=0 时，两个“人”都在起点 (0,0)，能摘到的樱桃数是 grid[0][0]
        f[0][1][1] = grid[0][0];
        for (int t = 1; t < n * 2 - 1; t++) {
            for (int j = max(t - n + 1, 0); j <= min(t, n - 1); j++) {
                // t > n-1   从 t-(n-1) - n-1   t < n-1  从 0 - t
                if (grid[t - j][j] < 0) continue;

                for (int k = j; k <= min(t, n - 1); k++) {
                    if (grid[t - k][k] < 0) continue;
                    f[t][j + 1][k + 1] = max({f[t - 1][j + 1][k + 1], f[t - 1][j + 1][k], f[t - 1][j][k + 1], f[t - 1][j][k]}) +
                                         grid[t - j][j] + (k != j ? grid[t - k][k] : 0);
                }
            }
        }
        return max(f[n * 2 - 2][n][n], 0);
    }
};