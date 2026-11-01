/*
 * @lc app=leetcode.cn id=695 lang=cpp
 *
 * [695] 岛屿的最大面积
 */

// @lc code=start
class Solution {
private:
    int dfs(vector<vector<int>>& grid, int x, int y) {
        if(x < 0 || x >= grid.size() || y < 0 \
            || y >= grid[0].size() || grid[x][y] == 0)
            return 0;
        int sum = 1;
        grid[x][y] = 0; // 标记已访问
        // 上下左右递归搜索
        sum += dfs(grid, x - 1, y);
        sum += dfs(grid, x + 1, y);
        sum += dfs(grid, x, y - 1);
        sum += dfs(grid, x, y + 1);
        return sum;
    }

public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        if(m == 0) return 0;
        int res = 0;
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if(grid[i][j] == 1)
                    res = max(dfs(grid, i, j), res);
            }
        }
        return res;
    }
};
// @lc code=end

