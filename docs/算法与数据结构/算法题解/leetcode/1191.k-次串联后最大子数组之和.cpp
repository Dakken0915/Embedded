/*
 * @lc app=leetcode.cn id=1191 lang=cpp
 *
 * [1191] K 次串联后最大子数组之和
 */

// @lc code=start
class Solution {
public:
    int maxSubArray(vector<int>& arr, int k) {
        int res = 0;
        int dp = 0;
        while(k--) {
            for(int x : arr) {
                dp = max(dp, 0) + x;
                res = max(res, dp);
            }
        }
        return res;
    }
    // second  Kadane 算法
    int kConcatenationMaxSum(vector<int>& arr, int k) {
        const int MOD = 1e9 + 7;
        if (k == 1)
            return maxSubArray(arr, 1); // arr 的最大子数组和

        // 防止一部分在第 1 个 arr 末尾，一部分在第 2 个 arr 开头
        long long ans = maxSubArray(arr, 2); // arr+arr 的最大子数组和
        int s = reduce(arr.begin(), arr.end());
        ans += 1LL * max(s, 0) * (k - 2);
        
        return ans % MOD;
    }
};
// @lc code=end

class Solution {
public:
    int kConcatenationMaxSum(vector<int>& arr, int k) {
        const int MOD = 1000000007;
        int n = arr.size();
        // if (n == 0) return 0;

        long long totalSum = 0; // 整个单数组arr所有元素的总和
        long long maxSub = 0, curKadane = 0;
        long long maxPre = 0, preSum = 0;
        long long maxSuf = 0, sufSum = 0;

        // 单次遍历：总和、单数组最大子段、最大前缀和
        for (int x : arr) {
            totalSum += x;

            curKadane = max((long long)x, curKadane + x);
            
            maxSub = max(maxSub, curKadane);
            preSum += x;
            maxPre = max(maxPre, preSum);
        }

        // 反向遍历求最大后缀和
        for (int i = n - 1; i >= 0; i--) {
            sufSum += arr[i];
            maxSuf = max(maxSuf, sufSum);
        }

        long long ans;
        if (k == 1) {
            ans = maxSub;
        } else {
            if (totalSum > 0) {
                // 数组总和为正，中间k-2段全部可取
                ans = max(maxSub, maxPre + maxSuf + totalSum * (k - 2));
            } else {
                // 总和为负，只取前后两段拼接
                ans = max(maxSub, maxPre + maxSuf);
            }
        }
        // 允许选取空数组，答案最小为0
        ans = max(ans, 0LL);
        return ans % MOD;
    }
};


// 时间复杂度 O(n⋅k) 超出限制
class Solution {
public:
    int kConcatenationMaxSum(vector<int>& arr, int k) {
        if(arr.empty()) return 0;
        const int MOD = 1e9 + 7;
        long long maxVal = 0;
        int n = arr.size();
        vector<long long> dp(n, 0);
        for(int i = 1; i <= k; i++) {
            for(int j = 0; j < n; j++) {
                if((i * n + j) == 0) 
                    dp[j] = dp[0];
                else if(j > 0)
                    dp[j] = max<long long>(arr[j], dp[j - 1] + arr[j]);
                else if(j == 0)
                    dp[j] = max<long long>(arr[j], dp[n - 1] + arr[j]);
                    maxVal = max(maxVal, dp[j]);
            }
        }
        return (int)(maxVal % MOD);
    }
};