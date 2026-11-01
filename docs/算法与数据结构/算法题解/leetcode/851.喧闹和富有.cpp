/*
 * @lc app=leetcode.cn id=851 lang=cpp
 *
 * [851] 喧闹和富有
 */

// @lc code=start
class Solution {
public:
    vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) {
        int n = quiet.size();

        vector<vector<int>> richGraph(n);
        vector<int> indeg(n);
        // 建图
        for(auto& x : richer) {
            int less = x[1];
            int more = x[0];
            richGraph[more].push_back(less);
            indeg[less]++;
        }

        queue<int> que;
        for(int i = 0; i < n; i++)
            if(indeg[i] == 0)
                que.push(i);

         vector<int> dp(n);
        for(int i = 0; i < n; i++) dp[i] = i; //初始候选是自己

        while(!que.empty()) {
            int x = que.front();
            que.pop();
            for(int y : richGraph[x]) {
                // 节点入队
                indeg[y]--;
                if(indeg[y] == 0)
                    que.push(y);
                    
                // 从更有钱的往前刷
                if(quiet[dp[y]] > quiet[dp[x]])
                    dp[y] = dp[x];
            }
        }
        return dp;
    }
};
// @lc code=end

