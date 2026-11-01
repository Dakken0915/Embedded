/*
 * @lc app=leetcode.cn id=3592 lang=cpp
 *
 * [3592] 硬币面值还原
 */

// @lc code=start

// 告诉 dp 数组，还原数组
class Solution {
public:
    vector<int> findCoins(vector<int>& numWays) {
        int n = numWays.size();
        vector<int> dp(n + 1, 0);
        dp[0] = 1;
        vector<int> ans;
        for(int i = 1; i <= n; i++) {
            int w = dp[i];
            if(w == numWays[i - 1]) continue;
            if(w != numWays[i - 1] - 1) return {};
            ans.push_back(i);

            // 现在得到了一个大小为 i 的物品，用 i 计算完全背包
            for(int j = i; j <= n; j++)
                dp[j] += dp[j - i];
        }
        return ans;
    }
};
// @lc code=end

