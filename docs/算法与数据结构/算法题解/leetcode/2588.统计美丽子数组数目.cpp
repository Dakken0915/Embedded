/*
 * @lc app=leetcode.cn id=2588 lang=cpp
 *
 * [2588] 统计美丽子数组数目
 */

// @lc code=start
class Solution {
public:
    long long beautifulSubarrays(vector<int>& nums) {
        // 子数组内每一位都有偶数个1 就是美丽子数组
        vector<int> prev(nums.size() + 1, 0);
        prev[0] = 0;
        unordered_map<int, long long> map;
        map[0] = 1;
        long long res = 0;

        for(int i = 0; i < nums.size(); i++) {
            prev[i + 1] = prev[i] ^ nums[i];
            res += map[prev[i + 1]];
            map[prev[i + 1]]++;
        }
        return res;
    }
};
// @lc code=end

