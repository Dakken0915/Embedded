/*
 * @lc app=leetcode.cn id=684 lang=cpp
 *
 * [684] 冗余连接
 */

// @lc code=start
class Solution {
public:
    int Find(vector<int>& parent, int index) {
        while(parent[index] != index)
            index = parent[index];
        return index;
    }

    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        int n = edges.size();
        vector<int> fa(n + 1, 0);  // 元
        for(int i = 0; i < n; i++)
            fa[i] = i;

        for(auto& edge : edges) {
            if(Find(fa, edge[0]) == Find(fa, edge[1])) // 说明形成环
                return edge;
            fa[Find(fa, edge[0])] = Find(fa, edge[1]);
        }
        return vector<int>{};
    }
};
// @lc code=end

