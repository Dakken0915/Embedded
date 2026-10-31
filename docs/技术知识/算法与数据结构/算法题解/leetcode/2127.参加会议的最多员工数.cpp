/*
 * @lc app=leetcode.cn id=2127 lang=cpp
 *
 * [2127] 参加会议的最多员工数
 */

// @lc code=start
class Solution {
public:
    int maximumInvitations(vector<int> &favorite) {
        int n = favorite.size();
        vector<int> indeg(n);

        for(int f : favorite)
            indeg[f]++;

        queue<int> que;
        for(int i = 0; i < n; i++) {
            if(indeg[i] == 0)
                que.push(i);
        }

        vector<int> max_depth(n, 1);  // 记录枝的长度
        while(!que.empty()) {  // 剪枝操作
            int x = que.front();
            que.pop();
            int y = favorite[x];
            indeg[y]--;
            max_depth[y] = max_depth[x] + 1;
            if(indeg[y] == 0)
                que.push(y);
        }

        int max_ring_len = 0;
        int sum_chain_size = 0;   // 所有二元环凑出来的最大参会人数

        for(int i = 0; i < n; i++) {
            if(indeg[i] == 0) continue;
            // 找到环
            int x = i;
            int ring_size = 0;
            while(indeg[x]) {
                indeg[x] = 0;
                ring_size++;
                x = favorite[x];  // 找下一项
            }

            if(ring_size == 2)  // 基环长度为 2
                // i是入环节点，二元环 favorite[i] 也是入环节点
                sum_chain_size += max_depth[i] + max_depth[favorite[i]]; // 累加两条最长链的长度
            else
                max_ring_len = max(max_ring_len, ring_size); // 取所有基环长度的最大值
        }

        return max(max_ring_len, sum_chain_size);
    }
};
// @lc code=end

// 示例
class Solution {
public:
    int maximumInvitations(vector<int> &favorite) {
        int n = favorite.size();
        vector<int> deg(n);
        for (int f: favorite) {
            deg[f]++; // 统计基环树每个节点的入度
        }

        vector<int> max_depth(n, 1);
        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (deg[i] == 0) {
                q.push(i);
            }
        }
        while (!q.empty()) { // 拓扑排序，剪掉图上所有树枝
            int x = q.front();
            q.pop();
            int y = favorite[x]; // x 只有一条出边
            max_depth[y] = max_depth[x] + 1;
            if (--deg[y] == 0) {
                q.push(y);
            }
        }

        int max_ring_size = 0, sum_chain_size = 0;
        for (int i = 0; i < n; i++) {
            if (deg[i] == 0) continue;

            // 遍历基环上的点
            deg[i] = 0; // 将基环上的点的入度标记为 0，避免重复访问
            int ring_size = 1; // 基环长度
            for (int x = favorite[i]; x != i; x = favorite[x]) {
                deg[x] = 0; // 将基环上的点的入度标记为 0，避免重复访问
                ring_size++;
            }

            if (ring_size == 2) { // 基环长度为 2
                sum_chain_size += max_depth[i] + max_depth[favorite[i]]; // 累加两条最长链的长度
            } else {
                max_ring_size = max(max_ring_size, ring_size); // 取所有基环长度的最大值
            }
        }
        return max(max_ring_size, sum_chain_size);
    }
};