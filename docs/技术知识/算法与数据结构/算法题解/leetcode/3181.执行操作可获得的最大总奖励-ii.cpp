/*
 * @lc app=leetcode.cn id=3181 lang=cpp
 *
 * [3181] 执行操作可获得的最大总奖励 II
 */

// @lc code=start
class Solution {
public:
    int maxTotalReward(vector<int>& rewardValues) {
        sort(rewardValues.begin(), rewardValues.end());
        auto last = unique(rewardValues.begin(), rewardValues.end());
        rewardValues.erase(last, rewardValues.end());  // 去重
        
        int n = rewardValues.size();
        int m = rewardValues[n - 1];

        bitset<100000> f{1};
        for(int v : rewardValues) {  // item
           int shift = f.size() - v;
            // 左移 shift 再右移 shift，把所有 >= v 的比特位置 0
            // f |= f << shift >> shift << v;
            f |= f << shift >> (shift - v); // 简化上式
        }

        for (int i = m * 2 - 1;; i--)
            if (f[i])
                return i;
    }
};
// @lc code=end

// 大数据超时
class Solution {
public:
    int maxTotalReward(vector<int>& rewardValues) {
        sort(rewardValues.begin(), rewardValues.end());
        auto last = unique(rewardValues.begin(), rewardValues.end());
        rewardValues.erase(last, rewardValues.end());  // 去重
        
        int n = rewardValues.size();
        int m = rewardValues[n - 1];

        vector<int> dp(2 * m, 0);
        dp[0] = 1;
        for(int val : rewardValues) {  // item
            if(dp[2 * m - 1] == 1) break;   // 已经拿到理论最大值
            for(int j = val - 1; j >= 0; j--) { // = contain
                if(dp[j])
                    dp[j + val] = 1;
            }
        }

        for(int i = 2 * m - 1; i >= 0; i--)
            if(dp[i])
                return i;
        return 0;
    }
};