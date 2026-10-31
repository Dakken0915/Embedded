/*
 * @lc app=leetcode.cn id=1284 lang=cpp
 *
 * [1284] 转化为全零矩阵的最少反转次数
 */

// @lc code=start
class Solution {
public:
    // 矩阵 -> 10进制数
    int encode(vector<vector<int>>& mat, int m, int n) {
        int x = 0;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                x = x * 2 + mat[i][j];
            }
        }
        return x;
    }

    // 10进制数 -> 矩阵
    vector<vector<int>> decode(int x, int m, int n) {
        vector<vector<int>> mat(m, vector<int>(n));
        for (int i = m - 1; i >= 0; --i) {
            for (int j = n - 1; j >= 0; --j) {
                mat[i][j] = x & 1;
                x >>= 1;
            }
        }
        return mat;
    }

    int dirs[5][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {0, 0}};
    // 点击坐标(i,j)，翻转该位置以及上下左右五个格子的 0/1 状态
    void convert(vector<vector<int>>& mat, int m, int n, int i, int j) {
        for (int k = 0; k < 5; ++k) {
            int i0 = i + dirs[k][0], j0 = j + dirs[k][1];
            if (i0 >= 0 && i0 < m && j0 >= 0 && j0 < n) {
                mat[i0][j0] ^= 1;
            }
        }
    }

    int minFlips(vector<vector<int>>& mat) {
        // 将矩阵状态存储为十进制数，根据状态 BFS
        int m = mat.size();
        int n = mat[0].size();

        int start = encode(mat, m, n);
        if(start == 0) return 0;

        queue<int> que;
        unordered_set<int> vis;
        vis.insert(start);
        que.push(start);

        int step = 0;
        while (!que.empty()) {
            int size = que.size();
            step++;
            // 一层一层计算步数
            for(int k = 0; k < size; k++) {
                int x = que.front();
                que.pop();
                vector<vector<int>> cur = decode(x, m, n);
                // 枚举每个点
                for(int i = 0; i < m; i++) {
                    for(int j = 0; j < n; j++) {
                        vector<vector<int>> copy = cur;
                        // 转换矩阵
                        convert(copy, m, n, i, j);
                        // 转换成十进制数
                        int y = encode(copy, m, n);
                        if(y == 0) return step;

                        // 第一次发现此状态
                        if(vis.find(y) == vis.end()) {
                            que.push(y);
                            vis.insert(y);
                        }
                    }
                }
            }
        }
        return -1;
    }
};
// @lc code=end

