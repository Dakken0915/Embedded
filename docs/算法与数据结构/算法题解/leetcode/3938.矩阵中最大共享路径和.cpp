/*
 * @lc app=leetcode.cn id=3938 lang=cpp
 *
 * [3938] 矩阵中最大共享路径和
 */

// @lc code=start
class Solution {
private:
    int maxSubArray(vector<int> arr) {
        // ans 不能赋初值 arr[0]，因为答案要保证两个单元格以上
        int ans = INT_MIN;
        int temp = arr[0];
        for(int i = 1; i < arr.size(); i++) {
            // 这里注意要保证至少两个单元格
            ans = max(ans, temp + arr[i]);
            temp = max(temp, 0) + arr[i];
        }
        return ans;
    }

public:
    int maxScore(vector<vector<int>>& grid) {
        int m = grid.size();
        int n = grid[0].size();
        // 若共享单元格只有一个，必定不在边界上
        int res = INT_MIN;
        for(int i = 1; i < m - 1; i++)
            for(int j = 1; j < n - 1; j++)
                res = max(res, grid[i][j]);

        // 单元格数量 >= 2 的情况
        // 每行的最大子数组
        for(auto& row : grid)
            res = max(res, maxSubArray(row));

        // 每列的最大子数组
        for(int j = 0; j < n; j++) {
            vector<int> col;
            for(int i = 0; i < m; i++)
                col.push_back(grid[i][j]);
            res = max(res, maxSubArray(col));
        }
        return res;
    }
};
// @lc code=end

