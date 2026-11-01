/*
 * @lc app=leetcode.cn id=3287 lang=cpp
 *
 * [3287] 求出数组中最大序列值
 */

// @lc code=start
class Solution {
public:
    int maxValue(vector<int>& nums, int k) {
        int n = nums.size();

        // dp[t]:选恰好t个元素，所有OR值，vector<int>，无重复
        auto getdp = [&](vector<int>& a) {
            int m = a.size();
            vector<vector<vector<int>>> res(m + 1);
            vector<vector<int>> dp(k + 1);
            dp[0].push_back(0);

            for (int i = 0; i < m; i++) {
                // ndp直接复制dp的vector，不用unordered_set
                vector<vector<int>> ndp = dp;
                int v = a[i];
                for (int t = k; t >= 1; t--) {
                    for (int x : dp[t - 1]) {
                        int new_or = x | v;
                        // 去重：ndp[t]里没有才push
                        bool exist = false;
                        for (int y : ndp[t]) {
                            if (y == new_or) {
                                exist = true;
                                break;
                            }
                        }
                        if (!exist) {
                            ndp[t].push_back(new_or);
                        }
                    }
                }
                dp.swap(ndp);
                res[i + 1] = dp;
            }
            return res;
        };

        auto pre = getdp(nums);
        reverse(nums.begin(), nums.end());
        auto sufrev = getdp(nums);
        reverse(sufrev.begin(), sufrev.end());

        int ans = 0;
        for (int pos = k; pos <= n - k; pos++) {
            const auto& A = pre[pos][k];
            const auto& B = sufrev[pos][k];
            for (int a : A) {
                for (int b : B) {
                    ans = max(ans, a ^ b);
                }
            }
        }
        return ans;
    }
};
// @lc code=end
