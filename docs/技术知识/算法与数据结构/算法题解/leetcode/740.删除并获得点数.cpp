/*
 * @lc app=leetcode.cn id=740 lang=cpp
 *
 * [740] 删除并获得点数
 */

// @lc code=start
class Solution {
public:
    int deleteAndEarn(vector<int>& nums) {
        // 转换为打家劫舍问题，值就是房子号码，不能打劫相邻房子
        unordered_map<int, int> mp;
        int maxVal = 0;
        for(int num : nums) {
            mp[num]++;
            maxVal = max(maxVal, num);
        }

        // 获得打家劫舍问题数组
        vector<int> array(maxVal, 0);
        for(int i = 0; i < maxVal; i++)
            array[i] = mp[i + 1];

        if(!array.size()) return 0;
        else if(array.size() == 1) return array[0];

        vector<int> dp(maxVal, 0);
        dp[0] = array[0];
        dp[1] = max(2 * array[1], dp[0]);
        for(int i = 2; i < maxVal; i++)
            dp[i] = max(dp[i - 2] + (i + 1) * array[i], dp[i - 1]);

        return dp[maxVal - 1];
    }
};
// @lc code=end

