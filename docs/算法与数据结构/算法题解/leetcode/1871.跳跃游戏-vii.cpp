/*
 * @lc app=leetcode.cn id=1871 lang=cpp
 *
 * [1871] 跳跃游戏 VII
 */

// @lc code=start

// 同向双指针解法
class Solution {
public:
    bool canReach(string s, int minJump, int maxJump) {
        int n = s.size();
        vector<bool> dp(n, false);
        dp[0] = true;
        int j = 1;  // 下标0 初始可达，不用处理
        for(int i = 0; i < n && j < n; i++) {
            // 只有当前位置i能停留、并且可达，才可以向外跳跃
            if(dp[i] == true && s[i] == '0') {
                j = max(j, i + minJump);
                // 向右标计可达点
                for(; j <= min(n - 1, i + maxJump); j++)
                    dp[j] = true;
            }
        }
        return dp[n - 1] && s[n - 1] == '0';
    }
};
// @lc code=end

// 动态规划 + 前缀和解法
class Solution {
public:
    bool canReach(string s, int minJump, int maxJump) {
        int n = s.size();
        vector<bool> dp(n, false);
        vector<int> pre(n + 1, 0); // pre[i] 代表 dp[0] ~ dp[i-1] 的可达总数
        dp[0] = true;   // 第一个位置可达
        pre[1] = 1;

        for(int i = 1; i < n; i++) {
            if(s[i] == '1') {
                pre[i + 1] = pre[i];
                continue;
            }
            int L = max(0, i - maxJump);
            int R = i - minJump;
            if(R >= 0) {
                dp[i] = (pre[R + 1] - pre[L]) > 0;
                pre[i+1] = pre[i] + (dp[i] ? 1 : 0);
            }
        }
        return dp[n - 1];
    }
};