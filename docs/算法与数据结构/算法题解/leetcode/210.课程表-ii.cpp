/*
 * @lc app=leetcode.cn id=210 lang=cpp
 *
 * [210] 课程表 II
 */

// @lc code=start
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> graph(numCourses);
        vector<int> indeg(numCourses);
        for(auto& e : prerequisites) {
            int from = e[1];
            int to = e[0];
            graph[from].push_back(to);
            indeg[to]++;
        }

        queue<int> que;
        // 将所有入度为 0 的入队
        for(int i = 0; i < numCourses; i++)
            if(indeg[i] == 0)
                que.push(i);

        vector<int> res;
        while(!que.empty()) {
            int cur = que.front();
            que.pop();
            res.push_back(cur);
            for(int x : graph[cur]) {
                indeg[x]--;
                if(indeg[x] == 0) // 将可以修的课程入队
                    que.push(x);
            }
        }

        for(int x : indeg)
            if(x != 0) // 只要有没办法修的课程就返回空
                return {};

        return res;
    }
};
// @lc code=end

