/*
 * @lc app=leetcode.cn id=1523 lang=cpp
 *
 * [1523] 在区间范围内统计奇数数目
 */

// @lc code=start
class Solution {
public:
    int countOdds(int low, int high) {
        return (high + 1) / 2 - (low) / 2;
    }
};
// @lc code=end

