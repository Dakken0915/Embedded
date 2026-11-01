/*
 * @lc app=leetcode.cn id=1306 lang=cpp
 *
 * [1306] 跳跃游戏 III
 */

// @lc code=start
class Solution {
public:
    vector<bool> vis;

    int dfs(vector<int>& arr, int cur) {
        vis[cur] = true;
        int val = arr[cur];
        if(val == 0) return 1;


        int res = 0;
        if(cur - val >= 0 && vis[cur - val] == false)
            res += dfs(arr, cur - val);
        if(cur + val < arr.size() && vis[cur + val] == false)
            res += dfs(arr, cur + val);
        return res;
    }

    bool canReach(vector<int>& arr, int start) {
        int n = arr.size();
        vis.assign(n, false);
        return dfs(arr, start) >= 1;
    }
};
// @lc code=end

