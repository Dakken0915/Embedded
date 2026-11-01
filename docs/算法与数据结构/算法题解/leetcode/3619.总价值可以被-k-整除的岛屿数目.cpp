/*
 * @lc app=leetcode.cn id=3619 lang=cpp
 *
 * [3619] 总价值可以被 K 整除的岛屿数目
 */

// @lc code=start
class Solution {
private:
    long long dfs(vector<vector<int>>& grid, int x, int y) {
        if(x < 0 || x >= grid.size() || y < 0 ||\
            y >= grid[0].size() || grid[x][y] == 0)
            return 0;
        long long sum = grid[x][y];
        grid[x][y] = 0;
        sum += dfs(grid, x - 1, y);
        sum += dfs(grid, x + 1, y);
        sum += dfs(grid, x, y - 1);
        sum += dfs(grid, x, y + 1);
        return sum;
    }
    
public:
    int countIslands(vector<vector<int>>& grid, int k) {
        int count = 0;
        long long value = 0;
        for(int i = 0; i < grid.size(); i++) {
            for(int j = 0; j < grid[0].size(); j++) {
                if(grid[i][j] > 0){
                    value = dfs(grid, i, j);
                    if(value % k == 0)
                        count++;
                }
            }
        }
        return count;
    }
};
// @lc code=end

