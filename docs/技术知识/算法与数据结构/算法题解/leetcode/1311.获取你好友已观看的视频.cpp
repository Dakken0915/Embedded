/*
 * @lc app=leetcode.cn id=1311 lang=cpp
 *
 * [1311] 获取你好友已观看的视频
 */

// @lc code=start
class Solution {
public:
    vector<string> watchedVideosByFriends(vector<vector<string>>& watchedVideos, vector<vector<int>>& friends, int id, int level) {
        // friends 就是 graph
        int n = friends.size();

        unordered_map<string, int> count;

        // BFS 过程
        vector<int> dis(n, -1);
        queue<int> que;
        que.push(id);
        dis[id] = 0;
        while(!que.empty()) {
            int x = que.front();
            que.pop();
            for(int y : friends[x]) {
                // 第一次访问时
                if(dis[y] == -1) {
                    que.push(y);
                    dis[y] = dis[x] + 1;
                    // 如果属于该 level, 统计视频
                    if(dis[y] == level) {
                        for(string video : watchedVideos[y])
                            count[video]++;
                    }
                }
            }
        }
        vector<pair<string, int>> rec(count.begin(), count.end()); 
        // 重载比较器
        auto compare = [&](pair<string ,int>& a, pair<string ,int>& b) {
            if(a.second != b.second)
                return a.second < b.second;
            else
                return a.first < b.first; //次数相同，字符串字典序小放前面
        };
        sort(rec.begin(), rec.end(), compare);

        vector<string> res;
        for(auto& x : rec)
            res.push_back(x.first);

        return res;
    }
};
// @lc code=end

