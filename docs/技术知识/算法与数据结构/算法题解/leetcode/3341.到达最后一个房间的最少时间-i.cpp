/*
 * @lc app=leetcode.cn id=3341 lang=cpp
 *
 * [3341] 到达最后一个房间的最少时间 I
 */

// @lc code=start
class Solution {
public:

    int minTimeToReach(vector<vector<int>>& moveTime) {
        int n = moveTime.size();
        int m = moveTime[0].size();
        /* 图已知不用建图 */

        moveTime[0][0] = 0; // 第一个房间不用开

        vector<vector<int>> dis(n, vector<int>(m, INT_MAX));
        // 存距离，坐标
        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> minHeap;
        // 初始化起点
        dis[0][0] = 0;
        minHeap.push({0, 0, 0});

        // 4个方向
        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};

        while(!minHeap.empty()) {
            auto [dis_cur, x, y] = minHeap.top();
            minHeap.pop();
            if(dis_cur > dis[x][y]) continue;

            // 处理邻居最短路
            for(int i = 0; i < 4; ++i)
            {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if(nx < 0 || nx >= n || ny <0 || ny >= m) continue;
                // 标准公式
                int next_time = max(dis_cur, moveTime[nx][ny]) + 1;
                if(next_time < dis[nx][ny])
                {
                    dis[nx][ny] = next_time;
                    minHeap.emplace(next_time, nx, ny);
                }
            }
        }
        return dis[n - 1][m - 1];
    }
};
// @lc code=end

