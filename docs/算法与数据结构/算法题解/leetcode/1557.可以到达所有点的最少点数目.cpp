/*
 * @lc app=leetcode.cn id=1557 lang=cpp
 *
 * [1557] 可以到达所有点的最少点数目
 */

// @lc code=start
class Solution {
public:
    vector<int> findSmallestSetOfVertices(int n, vector<vector<int>>& edges) {
        // 直接找入度为 0 的节点将其返回
        vector<int> indeg(n);
        for(auto& edge : edges) {
            int from = edge[0];
            int to = edge[1]; 
            indeg[to]++;
        }
        vector<int> res;
        for(int i = 0; i < n; i++)
            if(indeg[i] == 0)
                res. push_back(i);

        return res;
    }
};
// @lc code=end

