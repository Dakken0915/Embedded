/*
 * @lc app=leetcode.cn id=2658 lang=cpp
 *
 * [2658] 网格图中鱼的最大数目
 */

// @lc code=start
class Solution {
private:
    int dfs(vector<vector<int>>& grid, int x, int y) {
        if(x < 0 || x >= grid.size() \
            || y < 0 || y >= grid[0].size()) 
            return 0;
        if(grid[x][y] == 0) return 0;
        int sum = grid[x][y];
        grid[x][y] = 0;
        sum += dfs(grid, x + 1, y);
        sum += dfs(grid, x - 1, y);
        sum += dfs(grid, x, y + 1);
        sum += dfs(grid, x, y - 1);
        return sum;
    }

public:
    int findMaxFish(vector<vector<int>>& grid) {
        int res = 0;
        for(int i = 0; i < grid.size(); i++) {
            for(int j = 0; j < grid[0].size(); j++) {
                if(grid[i][j] > 0) {
                    res = max(res, dfs(grid, i, j));
                }
            }
        }
        return res;
    }
};
// @lc code=end

