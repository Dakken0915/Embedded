/*
 * @lc app=leetcode.cn id=3196 lang=cpp
 *
 * [3196] 最大化子数组的总成本
 */

// @lc code=start
class Solution {
public:
    // 对于长度超过 2 的子数组，可以继续分割为长度为 2 和 1 的子数组，而不改变成本之和
    long long maximumTotalCost(vector<int>& a) {
        int n = a.size();
        vector<long long> f(n + 1);
        f[1] = a[0];
        for (int i = 1; i < n; i++) {
            // 只考虑 两项的分割
            f[i + 1] = max(f[i] + a[i], f[i - 1] + a[i - 1] - a[i]);
        }
        return f[n];
    }
};
// @lc code=end

// 超时（机考解法）
class Solution {
public:
    long long maximumTotalCost(vector<int>& nums) {
        int n = nums.size();
        vector<long long> dp(n + 1, INT_MIN);
        dp[0] = 0;

        for(int i = 1; i <= n; i++) {
            long long sum = 0;
            for(int j = i - 1; j >= 0; j--) {
                // 枚举 [j, i) 的最优划分
                // long long sum = 0;
                // int cnt = 0;
                // for(int k = j; k < i; k++) {
                //     if(cnt % 2 == 0)
                //         sum += nums[k];
                //     else
                //         sum -= nums[k];
                //     cnt++;
                // }
                sum = nums[j] - sum;   // 核心递推，消去k循环
                dp[i] = max(dp[j] + sum, dp[i]);
            }
        }
        return dp[n];
    }
};