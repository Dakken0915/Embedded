/*
 * @lc app=leetcode.cn id=3493 lang=cpp
 *
 * [3493] 属性图
 */

// @lc code=start
class Solution {
public:
    int Find(vector<int>& fa, int x) {
        while(fa[x] != x)
            x = fa[x];
        return x;
    }

    int numberOfComponents(vector<vector<int>>& properties, int k) {
        int n = properties.size();
        int m = properties[0].size();
        vector<int> fa(n, 0);
        // 初始化并查集
        for(int i = 0; i < n; i++)
            fa[i] = i;

        // 合并并查集
        for(int i = 0; i < n - 1; i++) {
            unordered_set<int> set;
            for(int x : properties[i])
                if(set.find(x) == set.end())
                    set.insert(x);

            for(int j = i + 1; j < n; j++) {
                if(Find(fa, i) == Find(fa, j)) continue;

                // j行先去重
                unordered_set<int> set_j;
                for(int x : properties[j])
                    set_j.insert(x);

                int cnt = 0;
                for(int x : set_j) {
                    if(set.find(x) != set.end())
                        cnt++;
                }
                if(cnt >= k)
                    fa[Find(fa, j)] = Find(fa, i);
            }
        }

        // 计算联通分量
        unordered_set<int> rootSet;
        for(int i = 0; i < n; i++)
            rootSet.insert(Find(fa, i));

        return rootSet.size();
    }
};
// @lc code=end

