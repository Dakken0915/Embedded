/*
 * @lc app=leetcode.cn id=1512 lang=cpp
 *
 * [1512] 好数对的数目
 */

// @lc code=start
class Solution {
public:
    int numIdenticalPairs(vector<int>& nums) {
        unordered_map<int, int> map;
        int res = 0;
        for(int num : nums) {
            res += map[num];
            map[num]++;
        }
        return res;
    }
};
// @lc code=end

