/*
 * @lc app=leetcode.cn id=802 lang=cpp
 *
 * [802] 找到最终的安全状态
 */

// @lc code=start
class Solution {
public:
    vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
        // 构建反向图，找最终入度为 0的节点就是答案
        int n = graph.size();
        vector<int> outdeg(n, 0);         // 原图出度
        vector<vector<int>> rev(n);       // 反向图 rev[v]保存所有u，原图u→v

        // 1.建反向图 + 统计原图出度
        for(int u = 0; u < n; u++) {
            outdeg[u] = graph[u].size();
            for(int v : graph[u]) {
                rev[v].push_back(u);   // 原图u→v，反向图 v ← u，存v可以走到u
            }
        }

        queue<int> que;
        // 终端节点：原图出度等于0
        for(int i = 0; i < n; i++) {
            if(outdeg[i] == 0) {
                que.push(i);
            }
        }

        // 2.BFS拓扑
        while(!que.empty()) {
            int v = que.front();
            que.pop();
            // 遍历反向图所有前驱u（原图u→v）
            for(int u : rev[v]) {
                outdeg[u]--;
                if(outdeg[u] == 0) {
                    que.push(u);
                }
            }
        }

        // 3.收集答案，i从小到大遍历，结果天然升序
        vector<int> res;
        for(int i = 0; i < n; i++) {
            if(outdeg[i] == 0) {
                res.push_back(i);
            }
        }
        return res;
    }
};
// @lc code=end

