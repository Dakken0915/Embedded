/*
 * @lc app=leetcode.cn id=2140 lang=cpp
 *
 * [2140] 解决智力问题
 */

// @lc code=start
class Solution {
public:
    // second
    long long mostPoints(vector<vector<int>>& questions) {
        int n = questions.size();
        vector<long long> dp(n, 0);
        // 逆序dp
        for(int i = n - 1; i >= 0; i--) {
            int score = questions[i][0];
            int gap = questions[i][1];   // 做这道题要跳过的题数

            long long solve = score;  // 解决当前问题可以获得的分数
            if(i + gap + 1 < n)
                solve += dp[i + gap + 1];

            // 直接取上一个问题可以获得的最大分数
            long long skip = i+1 < n ? dp[i+1] : 0; // 跳过当前问题可以获得的分数

            dp[i] = max(solve, skip);
        }
        return dp[0];
    }
};
// @lc code=end

class Solution {
public:
    long long mostPoints(vector<vector<int>>& questions) {
        int n = questions.size();
        vector<long long> dp(n, 0);

        for(int i = n - 1; i >= 0; i--) {
            int points = questions[i][0];
            int brainpower = questions[i][1];

            // 计算解决当前问题后的下一个位置
            int nextIndex = i + brainpower + 1;

            // 跳过问题
            long long skip = i + 1 < n ? dp[i + 1] : 0;
            
            // 解决：如果 nextIndex 在数组范围内，加上 dp[nextIndex]，否则加 0
            long long solve = points;
            if (nextIndex < n)
                solve += dp[nextIndex];
            
            dp[i] = max(skip, solve);
        }
        return dp[0];
    }
};