/*
 * @lc app=leetcode.cn id=1984 lang=cpp
 *
 * [1984] 学生分数的最小差值
 */

// @lc code=start
class Solution {
public:
    int minimumDifference(vector<int>& nums, int k) {
        if(nums.size() <= 1) return 0;
        sort(nums.begin(), nums.end());
        int res = INT_MAX;  // #include <climits>
        for(int i = 0; i < nums.size(); i++) {
            if(i >= k - 1) {
                res = min(res, nums[i] - nums[i - k + 1]);
            }
        }
        return res;
    }
};
// @lc code=end

