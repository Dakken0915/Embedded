/*
 * @lc app=leetcode.cn id=624 lang=cpp
 *
 * [624] 数组列表中的最大距离
 */

// @lc code=start
class Solution {
public:
    int maxDistance(vector<vector<int>>& arrays) {
        int min_all = INT_MAX;
        int max_all = INT_MIN;
        int idx_min = 0, idx_max = 0;
        int m = arrays.size();

        // 遍历所有数组，找到全局最小、全局最大及其数组下标
        for (int i = 0; i < m; i++) {
            int cur_min = arrays[i][0];
            int cur_max = arrays[i].back();
            if (cur_min < min_all) {
                min_all = cur_min;
                idx_min = i;
            }
            if (cur_max > max_all) {
                max_all = cur_max;
                idx_max = i;
            }
        }

        // 情况1：最小和最大值不在同一个数组，直接返回差值
        if (idx_min != idx_max)
            return max_all - min_all;

        // 情况2：最小和最大值在同一个数组，需要计算两个候选最大值
        int candidate1 = INT_MIN; // min_all 与 其他数组最大值 的最大差
        int candidate2 = INT_MIN; // max_all 与 其他数组最小值 的最大差
        for (int i = 0; i < m; i++) {
            if (i == idx_min) continue; // 跳过min/max所在数组
            candidate1 = max(candidate1, arrays[i].back() - min_all);
            candidate2 = max(candidate2, max_all - arrays[i][0]);
        }
        return max(candidate1, candidate2);
    }
};
// @lc code=end

