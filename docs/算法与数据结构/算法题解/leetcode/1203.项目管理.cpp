/*
 * @lc app=leetcode.cn id=1203 lang=cpp
 *
 * [1203] 项目管理
 */

// @lc code=start
class Solution {
public:
    vector<int> topsort(vector<int>& deg, vector<vector<int>>& graph, vector<int>& items) {
        queue<int> que;
        for(auto& item : items) {
            if(deg[item] == 0)
                que.push(item);
        }
        vector<int> res;
        while(!que.empty()) {
            int x = que.front();
            que.pop();
            res.push_back(x);
            // 处理元素
            for(int y : graph[x]) {
                deg[y]--;
                if(deg[y] == 0)
                    que.push(y);
            }
        }
        return res.size() == items.size() ? res : vector<int>{};
    }

    vector<int> sortItems(int n, int m, vector<int>& group, vector<vector<int>>& beforeItems) {
        // 双重拓扑
        // 保存属于该小组的项目编号
        vector<vector<int>> groupItem(n + m);

        vector<vector<int>> groupGraph(n + m);
        vector<vector<int>> itemGraph(n);

        vector<int> groupDeg(n + m, 0);
        vector<int> itemDeg(n, 0);

        // 统计每个组，给拓扑排序使用
        vector<int> id;
        for (int i = 0; i < n + m; ++i) {
            id.push_back(i);
        }

        int leftId = m;
        // 给无从属项目分配一个组
        for (int i = 0; i < n; ++i) {
            if (group[i] == -1) {
                group[i] = leftId;
                leftId += 1;
            }
            groupItem[group[i]].push_back(i);
        }

        // 构建关系依赖图
        for(int i = 0; i < n; i++) { // i表示项目编号
            int curGroup = group[i];
            for(int x : beforeItems[i]) {
                int beforeGroup = group[x];
                if(curGroup == beforeGroup) {
                    // 构建项目依赖
                    itemGraph[x].push_back(i);
                    itemDeg[i]++;
                }
                else {
                    // 构建组依赖
                    groupGraph[beforeGroup].push_back(curGroup);
                    groupDeg[curGroup]++;
                }
            }
        }

        // 组间拓扑排序
        vector<int> groupSort = topsort(groupDeg, groupGraph, id);

        vector<int> res;
        // 组内拓扑排序
        for(auto& groupId : groupSort) {
            int size = groupItem[groupId].size();
            if (size == 0) continue;

            vector<int> curGroupItem = topsort(itemDeg, itemGraph, groupItem[groupId]);
            
            if (curGroupItem.size() == 0)
                return vector<int>{};  // 无法排序，直接返回空

            for(int i : curGroupItem)
                res.push_back(i);
        }
        return res;
    }
};
// @lc code=end

// 示例
class Solution {
public:
    vector<int> topSort(vector<int>& deg, vector<vector<int>>& graph, vector<int>& items) {
        queue<int> Q;
        for (auto& item: items) {
            if (deg[item] == 0) {
                Q.push(item);
            }
        }
        vector<int> res;
        while (!Q.empty()) {
            int u = Q.front(); 
            Q.pop();
            res.emplace_back(u);
            for (auto& v: graph[u]) {
                if (--deg[v] == 0) {
                    Q.push(v);
                }
            }
        }
        return res.size() == items.size() ? res : vector<int>{};
    }

    vector<int> sortItems(int n, int m, vector<int>& group, vector<vector<int>>& beforeItems) {
        vector<vector<int>> groupItem(n + m);

        // 组间和组内依赖图
        vector<vector<int>> groupGraph(n + m);
        vector<vector<int>> itemGraph(n);

        // 组间和组内入度数组
        vector<int> groupDegree(n + m, 0);
        vector<int> itemDegree(n, 0);
        
        vector<int> id;
        for (int i = 0; i < n + m; ++i) {
            id.emplace_back(i);
        }

        int leftId = m;
        // 给未分配的 item 分配一个 groupId
        for (int i = 0; i < n; ++i) {
            if (group[i] == -1) {
                group[i] = leftId;
                leftId += 1;
            }
            groupItem[group[i]].emplace_back(i);
        }
        // 依赖关系建图
        for (int i = 0; i < n; ++i) {
            int curGroupId = group[i];
            for (auto& item: beforeItems[i]) {
                int beforeGroupId = group[item];
                // 属于同一组
                if (beforeGroupId == curGroupId) {
                    itemDegree[i] += 1;
                    itemGraph[item].emplace_back(i);   
                } else {  // 属于不同组
                    groupDegree[curGroupId] += 1;
                    groupGraph[beforeGroupId].emplace_back(curGroupId);
                }
            }
        }

        // 组间拓扑关系排序
        vector<int> groupTopSort = topSort(groupDegree, groupGraph, id); 
        if (groupTopSort.size() == 0) {
            return vector<int>{};
        } 
        vector<int> ans;
        // 组内拓扑关系排序
        for (auto& curGroupId: groupTopSort) {
            int size = groupItem[curGroupId].size();
            if (size == 0) {
                continue;
            }
            vector<int> res = topSort(itemDegree, itemGraph, groupItem[curGroupId]);
            if (res.size() == 0) {
                return vector<int>{};
            }
            for (auto& item: res) {
                ans.emplace_back(item);
            }
        }
        return ans;
    }
};