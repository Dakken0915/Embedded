/*
 * @lc app=leetcode.cn id=52 lang=cpp
 *
 * [52] N 皇后 II
 */

// @lc code=start

// 位运算解法
class Solution {
private:
    int process(int limit, int col, int left, int right) {
        if(col == limit) return 1;  // 得到一种解法

        int pos = limit & (~(col | left | right));  // 得到限制
        int res = 0;
        while(pos != 0) {
            int mostRightOne = pos & (~pos + 1);   // 取出最右边的1
            pos = pos - mostRightOne;    // 去除一种可能

            // 注意位运算优先级
            res += process(limit, col | mostRightOne, 
                 (left | mostRightOne) << 1, 
                (right | mostRightOne) >> 1);
        }
        return res;
    }

public:
    int totalNQueens(int n) {
        if(n < 1 || n > 32) return 0;
        int limit = n == 32 ? -1 : (1 << n) - 1;   // 将前 n 位设置为 1
        return process(limit, 0, 0, 0);
    }
};
// @lc code=end

// 回溯解法
class Solution {
private:
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

    // record[i] = j 记录第i行的皇后放在第j列
    int process(vector<int>& record, int i, int n) {
        if(i == n) return 1;  // 到达终止行，得到一种可能性
        int res = 0;
        for(int j = 0; j < n; j++) {
            if(isValid(record, i, j)) {  // 校验合法性
                record[i] = j;
                res += process(record, i + 1, n);
            }
        }
        return res;
    }

public:
    int totalNQueens(int n) {
        if(n == 1) return 1;
        vector<int> record(n, 0);
        return process(record, 0, n);
    }
};