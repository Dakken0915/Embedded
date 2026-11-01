/*
 * @lc app=leetcode.cn id=525 lang=cpp
 *
 * [525] 连续数组
 */

// @lc code=start
class Solution {
public:
    int findMaxLength(vector<int>& nums) {
        int res = 0;
        vector<int> prevZero(nums.size() + 1, 0);
        vector<int> prevOne(nums.size() + 1, 0);
        unordered_map<int, int> diff_map;
        diff_map[0] = 0;

        for(int i = 0; i < nums.size(); i++) {
            prevZero[i + 1] = prevZero[i];
            prevOne[i + 1] = prevOne[i];

            if(nums[i]) prevOne[i + 1]++;
            else prevZero[i + 1]++;

            int diff = prevZero[i + 1] - prevOne[i + 1];

            // 在哈希表里找 1 和 0 差值一样大的下标
            if(diff_map.find(diff) != diff_map.end())
                res = max(res, i + 1 - diff_map[diff]);

            else diff_map[diff] = i + 1;
        }
        return res;
    }
};
// @lc code=end

