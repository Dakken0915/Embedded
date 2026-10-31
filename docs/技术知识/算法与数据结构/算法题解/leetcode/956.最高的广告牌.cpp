/*
 * @lc app=leetcode.cn id=956 lang=cpp
 *
 * [956] 最高的广告牌
 */

// @lc code=start
class Solution {
public:
    int tallestBillboard(vector<int>& rods) {
        int sum = reduce(rods.begin(), rods.end());

        // dp[i] 表示两段差分为i 的较长一段的长度
        vector<int> dp(sum + 1, INT_MIN);
        dp[0] = 0;
        for(int len : rods) {
            vector<int> ndp = dp;   // ndp：不选当前钢筋的状态
            for(int i = sum; i >= 0; i--) {
                // 数值更改用 dp ,更改后的新值存入 ndp  
                if(dp[i] == INT_MIN) continue;  // 跳过不合法的
                // 加到较短的一端
                if(len >= i)
                    ndp[len - i] = max(ndp[len - i], dp[i] - i + len);
                else
                    ndp[i - len] = max(ndp[i - len], dp[i]);
                // 加到较长一端
                if(len + i <= sum)
                    ndp[len + i] = max(ndp[len + i], dp[i] + len);
            }
            dp.swap(ndp);  // 交换指针
        }
        return dp[0];
    }
};
// @lc code=end

