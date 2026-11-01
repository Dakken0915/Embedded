/*
 * @lc app=leetcode.cn id=137 lang=cpp
 *
 * [137] 只出现一次的数字 II
 */

// @lc code=start
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        unordered_map<int, int> map;
        for(int x : nums)
            map[x]++;
        for(auto [x, count] : map)
            if(count == 1) return x;
        return 0;
    }
};
// @lc code=end

