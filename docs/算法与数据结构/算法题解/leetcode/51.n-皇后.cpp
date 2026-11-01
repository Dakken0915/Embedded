/*
 * @lc app=leetcode.cn id=51 lang=cpp
 *
 * [51] N 皇后
 */

// @lc code=start
class Solution {
private:
    vector<vector<string>> result;  // 存储所有解

    // 检查有效性
    int isValid(vector<int>& record, int line, int col) {
        for(int i = 0; i < line; i++) {
            if(col == record[i] 
            || abs(col - record[i]) == line - i)
            // #include <cstdlib>
                return 0;
        }
        return 1;
    }

    // 将record转换为棋盘字符串
    vector<string> buildBoard(vector<int>& record, int n) {
        vector<string> board(n, string(n, '.'));
        for(int i = 0; i < n; i++) {
            board[i][record[i]] = 'Q';
        }
        return board;
    }

    // 回溯主函数
    // record[i] = j 记录第i行的皇后放在第j列
    void process(vector<int>& record, int i, int n) {
        if(i == n) {
            result.push_back(buildBoard(record, n));
            return;
        }
        for(int j = 0; j < n; j++) {
            if(isValid(record, i, j)) {  // 校验合法性
                record[i] = j;
                process(record, i + 1, n);
            }
        }
    }

public:
    vector<vector<string>> solveNQueens(int n) {
        vector<int> record(n, 0);
        process(record, 0 , n);
        return result;
    }
};
// @lc code=end
