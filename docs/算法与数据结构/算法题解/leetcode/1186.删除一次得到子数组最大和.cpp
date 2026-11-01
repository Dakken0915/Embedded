/*
 * @lc app=leetcode.cn id=1186 lang=cpp
 *
 * [1186] 删除一次得到子数组最大和
 */

// @lc code=start
class Solution {
public:
    int maximumSum(vector<int>& arr) {
        int n = arr.size();
        vector<vector<int>> dp(n, vector<int>(2));
        dp[0][0] = arr[0];   // 未删除
        dp[0][1] = arr[0];   // 删除过
        int res = arr[0];
        for(int i = 1; i < n; i++) {
            // 选前一个未删除或截断
            dp[i][0] = max(dp[i - 1][0], 0) + arr[i];
            // 选前一个删除过加上当前值 或者 前一个未删除(删除该项)
            dp[i][1] = max(dp[i - 1][1] + arr[i], dp[i - 1][0]);
            res = max(res, max(dp[i][0], dp[i][1]));
        }
        return res;
    }
};
// @lc code=end

