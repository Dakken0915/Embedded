// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem850.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=850 lang=cpp
 *
 * [850] 矩形面积 II
 */

// @lc code=start
class Solution {
public:
    int rectangleArea(vector<vector<int>>& rectangles) {
        // 核心思路：将分界线划分出来的一块区域视为一格
        const int MOD = 1e9 + 7;
        vector<int> xs, ys;
        for(auto& x : rectangles) {
            xs.push_back(x[0]);
            xs.push_back(x[2]);
            ys.push_back(x[1]);
            ys.push_back(x[3]);
        }
        // 排序+去重
        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());

        int m = xs.size();
        int n = ys.size();

        // 计算差分数组
        vector diff(m + 2, vector<int>(n + 2, 0));
        for(auto& r : rectangles) {
            long long x1 = r[0], y1 = r[1], x2 = r[2], y2 = r[3];
            // 取出下标  lower_bound: 取出第一个 <=x or y 的数 
            int i1 = lower_bound(xs.begin(), xs.end(), x1) - xs.begin();
            int i2 = lower_bound(xs.begin(), xs.end(), x2) - xs.begin();
            int j1 = lower_bound(ys.begin(), ys.end(), y1) - ys.begin();
            int j2 = lower_bound(ys.begin(), ys.end(), y2) - ys.begin();
            diff[i1 + 1][j1 + 1]++;
            diff[i1 + 1][j2 + 1]--;
            diff[i2 + 1][j1 + 1]--;
            diff[i2 + 1][j2 + 1]++;
        }

        // 还原数组
        int res = 0;
        for(int i = 0; i < m - 1; i++) {
            for(int j = 0; j < n - 1; j++) {
                diff[i+1][j+1] += diff[i][j+1] + diff[i+1][j] - diff[i][j];

                if(diff[i+1][j+1] > 0) {
                    long long width = xs[i + 1] - xs[i];
                    long long height = ys[j + 1] - ys[j];
                    res = (res + width * height) % MOD;
                }
            }
        }
        return res % MOD;
    }
};
// @lc code=end
