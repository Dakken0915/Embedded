/*
 * @lc app=leetcode.cn id=1094 lang=cpp
 *
 * [1094] 拼车
 */

// @lc code=start
class Solution {
public:
    bool carPooling(vector<vector<int>>& trips, int capacity) {
        map<int, int> diff;
        for(auto& x : trips) {
            diff[x[1]] += x[0];
            diff[x[2]] -= x[0];
        }

        int num = 0;
        for(auto& [_, t] : diff) {
            num += t;
            if(num > capacity) return false;
        }
        return true;
    }
};
// @lc code=end

