/*
 * @lc app=leetcode.cn id=1139 lang=cpp
 *
 * [1139] 最大的以 1 为边界的正方形
 */

// @lc code=start
class Solution {
public:
    int largest1BorderedSquare(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        int res = 0;
        //=================== 预处理 ===================
        // 生成一个 right 矩阵，记录每个数右边有多少个1
        vector<vector<int>> right(m, vector<int>(n, 0));
        // 生成一个 down 矩阵，记录每个数下方有多少个1
        vector<vector<int>> down(m, vector<int>(n, 0));

        int cnt = 0;
        for(int i = 0; i < m; i++) {
            cnt = 0;
            for(int j = n - 1; j >= 0; j--) {
                right[i][j] = cnt;
                if(grid[i][j] == 1) {
                    cnt++;
                    res = 1;
                }
                else cnt = 0;
            }
        }
        if(!res) return 0;

        for(int i = 0; i < n; i++) {
            cnt = 0;
            for(int j = m - 1; j >= 0; j--) {
                down[j][i] = cnt;
                if(grid[j][i] == 1) cnt++;
                else cnt = 0;
            }
        }

        //=================== 判断 ====================
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if (grid[i][j] == 0) continue;
                // 当前位置能构成的最大正方形边长
                int maxK = min(right[i][j], down[i][j]);
                for (int k = maxK; k >= 1; k--) {
                    if (right[i + k][j] >= k && 
                        down[i][j + k] >= k) {
                        res = max(res, (k + 1) * (k + 1));
                        break;  // 找到当前格子能构成的最大正方形，跳出内层循环
                    }
                }
            }
        }
        return res;
    }
};
// @lc code=end

