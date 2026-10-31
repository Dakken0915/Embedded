/*
 * @lc app=leetcode.cn id=3286 lang=cpp
 *
 * [3286] 穿越网格图的安全路径
 */

// @lc code=start
class Solution {
public:
    static constexpr int dirs[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    bool findSafeWalk(vector<vector<int>>& grid, int health) {
        int m = grid.size();
        int n = grid[0].size();

        deque<pair<int, int>> dq;
        dq.push_back({0, 0});
        vector<vector<int>> cost(m, vector<int>(n, INT_MAX));
        cost[0][0] = grid[0][0];

        while(!dq.empty()) {
            int x = dq.front().first;
            int y = dq.front().second;
            dq.pop_front();

            // 处理结果
            if(cost[x][y] >= health) return false;
            if(x == m - 1 && y == n - 1) return true;

            for(int k = 0; k < 4; k++) {
                int x0 = x + dirs[k][0];
                int y0 = y + dirs[k][1];
                if(x0 >= 0 && x0 < m && y0 >= 0 && y0 < n) {
                    int w = grid[x0][y0];

                    // dp操作, 寻找消耗最少的路径
                    if(cost[x][y] + w <  cost[x0][y0]) {
                        cost[x0][y0] = cost[x][y] + w;

                        // 双端队列核心：松弛更新成功才入队
                        if(w == 0)
                            dq.push_front({x0, y0});
                        else
                            dq.push_back({x0, y0});

                    }
                }
            }
        }
        return false;
    }
};
// @lc code=end

