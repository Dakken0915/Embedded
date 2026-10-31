/*
 * @lc app=leetcode.cn id=1109 lang=cpp
 *
 * [1109] 航班预订统计
 */

// @lc code=start
class Solution {
public:
    vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
        map<int, int> diff;
        for(int i = 0; i < bookings.size(); i++) {
            int first = bookings[i][0];
            int last = bookings[i][1];
            int seats = bookings[i][2];
            diff[first-1] += seats;
            diff[last] -= seats;  // 注意这里下标
        }

        vector<int> ans(n, 0);
        ans[0] = diff[0];
        for(int i = 1; i < n; i++)
            ans[i] = ans[i - 1] + diff[i];

        return ans;
    }
};
// @lc code=end

