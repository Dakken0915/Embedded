/*
 * @lc app=leetcode.cn id=3427 lang=cpp
 *
 * [3427] 变长子数组求和
 */

// @lc code=start
class Solution {
public:
    int subarraySum(vector<int>& nums) {
        vector<int> s(nums.size() + 1, 0);
        for(int i = 0; i < nums.size(); i++)
            s[i + 1] = s[i] + nums[i];

        int res = 0;
        for(int i = 0; i < nums.size(); i++)
            res += s[i + 1] - s[max(0, i - nums[i])];
        return res;
    }
};
// @lc code=end

