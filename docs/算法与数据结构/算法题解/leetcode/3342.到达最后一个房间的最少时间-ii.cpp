/*
 * @lc app=leetcode.cn id=3342 lang=cpp
 *
 * [3342] 到达最后一个房间的最少时间 II
 */

// @lc code=start
class Solution {
public:
    static constexpr int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    int minTimeToReach(vector<vector<int>>& moveTime) {
        int n = moveTime.size();
        int m = moveTime[0].size();

        vector<vector<int>> dis(n, vector<int>(m, INT_MAX));
        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<>> minHeap;
        // 初始化起点
        moveTime[0][0] = 0;
        dis[0][0] = 0;
        minHeap.push({0, 0, 0});

        while(!minHeap.empty()) {
            auto [curdis, x, y] = minHeap.top();
            minHeap.pop();

            if(curdis > dis[x][y]) continue;

            for(int i = 0; i < 4; i++) {
                int x0 = x + dirs[i][0];
                int y0 = y + dirs[i][1];
                if(x0 < 0 || x0 >= n || y0 < 0 || y0 >= m) continue;
                int time = (x0 + y0) % 2 == 0 ? 2 : 1;
                int new_dis = max(curdis, moveTime[x0][y0]) + time;
                if(new_dis < dis[x0][y0]) {
                    dis[x0][y0] = new_dis;
                    minHeap.push({new_dis, x0, y0});
                }
            }
        }
        return dis[n - 1][m - 1];
    }
};
// @lc code=end

