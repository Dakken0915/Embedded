#include <iostream>

using namespace std;

/*
 * @lc app=leetcode.cn id=70 lang=cpp
 *
 * [70] 爬楼梯
 */

 // @lc code=start
class Solution {
public:
    int climbStairs(int n) {
        vector<int> dp(n + 1, 0);
        dp[0] = 1;
        dp[1] = 1;
        for (int i = 2; i < n + 1; i++)
            dp[i] = dp[i - 1] + dp[i - 2];
        return dp[n];
    }
};
// @lc code=end

int main() {
    Solution sol;
    int n;
    cin >> n;
    int res = sol.climbStairs(n);
    cout << res;
}