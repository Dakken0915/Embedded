/*
 * @lc app=leetcode.cn id=334 lang=cpp
 *
 * [334] 递增的三元子序列
 */

// @lc code=start
class Solution {
public:
    // 二分优化
    bool increasingTriplet(vector<int>& nums) {
        vector<int> g;
        for (int x : nums) {
            auto it = ranges::lower_bound(g, x);
            // 比 *it 小的数至少有两个
            if (it - g.begin() >= 2) { // LIS 长度至少是 3
                return true;
            }
            if (it == g.end()) { // >=x 的 g[j] 不存在
                g.push_back(x);
            } else {
                *it = x;  // 将找到的值换成更小的 x
            }
        }
        return false;
    }
};
// @lc code=end

// 超时
class Solution {
public:
    bool increasingTriplet(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n, 0);
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < i; j++) {
                if(nums[j] < nums[i])
                    dp[i] = max(dp[i], dp[j]);
            }
            dp[i] += 1;
            if(dp[i] >= 3) return true;
        }
        return false;
    }
};