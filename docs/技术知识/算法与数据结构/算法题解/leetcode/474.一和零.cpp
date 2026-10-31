/*
 * @lc app=leetcode.cn id=474 lang=cpp
 *
 * [474] 一和零
 */

// @lc code=start

// dp[i][j]的意义是：对于扫描过的字符串，只用i个0和j个1能存下来字符串数量
class Solution {
public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        // 创建二维数组，全部初始0
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

        // 逐个拿出来每个字符串
        for (auto &s : strs) {
            // 统计当前字符串有多少个0、多少个1
            int zero = count(s.begin(), s.end(), '0');
            int one = s.size() - zero;

            // 二维01背包：i、j 从大往小逆序遍历
            for (int i = m; i >= zero; i--) {
                for (int j = n; j >= one; j--) {
                    // 状态转移公式
                    dp[i][j] = max(dp[i][j], dp[i-zero][j-one] + 1);
                }
            }
        }
        return dp[m][n];
    }
};
// @lc code=end

// 递归：存在时间超限问题
class Solution {
private:
    // sum 存储已使用子串个数
    int dfs(vector<string>& strs, int i, int m, int n) {
        if(i < 0) return 0;

        int zero = count(strs[i].begin(), strs[i].end(), '0');
        int one = strs[i].size() - zero;

        int nochoose =  dfs(strs, i - 1, m, n);
        if(m < zero || n < one) 
            return nochoose;
        int choose = dfs(strs, i - 1, m - zero, n - one) + 1;

        return max(choose, nochoose);
    }

public:
    int findMaxForm(vector<string>& strs, int m, int n) {
        int num = strs.size();
        return dfs(strs, num - 1, m, n);
    }
};