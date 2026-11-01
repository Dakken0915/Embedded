/*
 * @lc app=leetcode.cn id=96 lang=cpp
 *
 * [96] 不同的二叉搜索树
 */

// @lc code=start

// 动态规划
class Solution {
public:
    int numTrees(int n) {
        vector<int> dp(n + 1, 0);
        dp[0] = 1;
        for(int i = 1; i <= n; i++)  // 节点数为 i 个的时候
            for(int j = 0; j <= i - 1; j++) // 左侧 j 个节点
                dp[i] += dp[j] * dp[i - 1 - j];
        return dp[n];
    }
};
// @lc code=end

class Solution {
public:
    int numTrees(int n) {
        if(n == 1 || n == 0) return 1;
        if(n == 2) return 2;
        
        int res = 0;
        for(int i = 0; i < n; i++) {
            int left = numTrees(i);
            int right = numTrees(n - 1 - i);
            res += left * right;
        }
        return res;
    }
};