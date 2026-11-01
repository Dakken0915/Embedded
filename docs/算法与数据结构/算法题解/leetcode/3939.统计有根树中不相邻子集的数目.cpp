/*
 * @lc app=leetcode.cn id=3939 lang=cpp
 *
 * [3939] 统计有根树中不相邻子集的数目
 */

// @lc code=start
class Solution {
public:
    int kGlobal;
    vector<vector<int>> g;
    vector<int> nums;
    const int MOD = 1e9 + 7;

    // 返回 pair<f0, f1>
    pair<vector<long long>, vector<long long>> dfs(int x) {
        int k = kGlobal;
        vector<long long> f0(k, 0);
        vector<long long> f1(k, 0);
        f0[0] = 1;
        f1[nums[x] % k] = 1;

        for (int y : g[x]) {
            auto [fy0, fy1] = dfs(y);

            // ========== nf0：不选x，y可以选或者不选 ==========
            vector<long long> nf0(k, 0);
            for (int i = 0; i < k; ++i) {
                long long v = (fy0[i] + fy1[i]) % MOD;
                if (v == 0) continue;
                for (int j = 0; j < k; ++j) {
                    int s = (i + j) % k;
                    nf0[s] = (nf0[s] + v * f0[j]) % MOD;
                }
            }

            // ========== nf1：选x，则y不能选，只能取fy0 ==========
            vector<long long> nf1(k, 0);
            for (int i = 0; i < k; ++i) {
                long long v = fy0[i];
                if (v == 0) continue;
                for (int j = 0; j < k; ++j) {
                    int s = (i + j) % k;
                    nf1[s] = (nf1[s] + v * f1[j]) % MOD;
                }
            }

            f0.swap(nf0);
            f1.swap(nf1);
        }
        return {move(f0), move(f1)};
    }

    int countValidSubsets(vector<int>& parent, vector<int>& nums_, int k) {
        int n = parent.size();
        g.assign(n, vector<int>());
        for (int i = 1; i < n; ++i) {
            g[parent[i]].push_back(i);
        }
        nums = nums_;
        kGlobal = k;

        auto [f0, f1] = dfs(0);
        // f0[0]+f1[0] 包含空集，要减1
        int ans = (f0[0] + f1[0] - 1 + MOD) % MOD;
        return ans;
    }
};
// @lc code=end

