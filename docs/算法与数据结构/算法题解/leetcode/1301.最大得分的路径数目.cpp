/*
 * @lc app=leetcode.cn id=1301 lang=cpp
 *
 * [1301] 最大得分的路径数目
 */

// @lc code=start
class Solution {
public:
    vector<int> pathsWithMaxScore(vector<string>& board) {
        int MOD = 1e9 + 7;
        int m = board.size();
        int n = board[0].size();

        vector<vector<int>> max_sum(m + 1, vector<int>(n + 1, INT_MIN));
        vector<vector<int>> ways(m + 1, vector<int>(n + 1, 0));
        max_sum[0][0] = 0;
        ways[0][0] = 1;
        // 初始化边界
        for(int i = 1; i <= m; i++) {
            for(int j = 1; j <= n; j++) {
                char ch = board[i - 1][j - 1];
                if(ch == 'X') continue;
                max_sum[i][j] = max({max_sum[i - 1][j], \
                    max_sum[i][j - 1], max_sum[i - 1][j - 1]});
                
                long long w = 0;
                if(max_sum[i][j] == max_sum[i - 1][j])
                    w += ways[i - 1][j];
                if(max_sum[i][j] == max_sum[i][j - 1])
                    w += ways[i][j - 1];
                if(max_sum[i][j] == max_sum[i - 1][j - 1])
                    w += ways[i - 1][j - 1];
                ways[i][j] = w % MOD;
                
                 // 加上当前格子的值
                if ('1' <= ch && ch <= '9')
                    max_sum[i][j] += ch - '0';
            }
        }
        return max_sum[m][n] < 0 ? vector{0, 0} : vector{max_sum[m][n], ways[m][n]};
    }
};
// @lc code=end

