/*
 * @lc app=leetcode.cn id=560 lang=cpp
 *
 * [560] 和为 K 的子数组
 */

// @lc code=start
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        vector<int> prev(nums.size() + 1, 0);
        prev[0] = 0;
        unordered_map<int, int> map;
        map[0] = 1;
        int res = 0;
        
        for(int i = 0; i < nums.size(); i++) {
            prev[i + 1] = prev[i] + nums[i];
            res += map[prev[i + 1] - k];
            map[prev[i + 1]]++;
        }
        return res;
    }
};
// @lc code=end

