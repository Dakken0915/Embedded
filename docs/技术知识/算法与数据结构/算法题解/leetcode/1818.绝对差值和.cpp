/*
 * @lc app=leetcode.cn id=1818 lang=cpp
 *
 * [1818] 绝对差值和
 */

// @lc code=start
#include <algorithm>
class Solution {
public:
    int minAbsoluteSumDiff(vector<int>& nums1, vector<int>& nums2) {
        int MOD = 1e9 + 7;
        vector<int> rec;
        rec = nums1;
        sort(rec.begin(), rec.end());
        long long ans = 0;
        int maxReduce = 0;  // 最多能减小的差值
        for(int i = 0; i < nums1.size(); i++) {
            // 计算常规差值和
            ans += abs(nums1[i] - nums2[i]);

            auto it = lower_bound(rec.begin(), rec.end(), nums2[i]);
            int x = INT_MAX;
            // 既不在末尾也不在开头，会同时走两个分支
            if(it != rec.end())
                x = min(x, abs(*it - nums2[i]));
            if(it != rec.begin())
                x = min(x, abs(*prev(it) - nums2[i]));

            // 记录可能减小的最大值, 如果反而增大了, 直接舍弃掉
            maxReduce = max(maxReduce, abs(nums1[i] - nums2[i]) - x);
        }
        return (ans - maxReduce) % MOD;
    }
};
// @lc code=end

