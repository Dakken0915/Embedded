/*
 * @lc app=leetcode.cn id=3825 lang=cpp
 *
 * [3825] 按位与结果非零的最长上升子序列
 */

// @lc code=start
class Solution {
public:
    int longestSubsequence(vector<int>& nums) {
        vector<int> sorelanuxi = nums;
        int ans = 0;
        for(int b = 0; b <= 30; b++)
        {
            vector<int> arr;
            for(int x : sorelanuxi)
                if((x >> b) & 1)
                    arr.push_back(x);

            vector<int> tails;
            for(int v : arr)
            {
                auto it = lower_bound(tails.begin(), tails.end(), v);
                if(it == tails.end())
                    tails.push_back(v);
                else
                    *it = v;
            }
            ans = max(ans, (int)tails.size());
        }
        return ans;
    }
};
// @lc code=end

// dp 没办法保存多个结果
class Solution {
public:
    int longestSubsequence(vector<int>& nums) {
        int m = nums.size();

        // 记录相与的结果和长度
        vector<pair<int, int>> dp(m, {0, 0});
        for(int i = 0; i < m; i++)
            dp[i].first = nums[i];

        for(int i = 0; i < m; i++) {
            for(int j = 0; j < i; j++) {
                if(nums[j] < nums[i] && (dp[j].first & nums[i]) != 0 
                    && dp[j].second > dp[i].second)
                    dp[i] = {dp[j].first & nums[i], dp[j].second};
            }
            dp[i].second += 1;
        }
        int res = 0;
        for(auto x : dp)
            res = max(res, x.second);
        return res;
    }
};