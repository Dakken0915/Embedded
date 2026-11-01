/*
 * @lc app=leetcode.cn id=721 lang=cpp
 *
 * [721] 账户合并
 */

// @lc code=start
class Solution {
public:
    unordered_map<string,vector<string>> graph;
    unordered_map<string,string> emailToName;
    unordered_set<string> vis;

    void dfs(string email, vector<string>& temp) {
        if(vis.count(email)) return;
        vis.insert(email);         // 标记已访问，漏掉会死递归
        temp.push_back(email);     // 将当前邮箱加入连通块集合

        for(string& neighbor : graph[email]) {
            dfs(neighbor, temp);
        }
    }

    vector<vector<string>> accountsMerge(vector<vector<string>>& accounts) {
        unordered_set<string> allEmail;  // 保存全部邮箱，用于遍历启动dfs

        // 构建映射关系 + 构建无向图
        for(auto &acc : accounts)
        {
            string name = acc[0];
            for(int i = 1; i < acc.size(); i++)
            {
                string e = acc[i];
                emailToName[e] = name;
                allEmail.insert(e);
                if(i > 1)
                {
                    string u = acc[1];
                    string v = acc[i];
                    graph[u].push_back(v);
                    graph[v].push_back(u);
                }
            }
        }

        vector<vector<string>> res;
        // 遍历全部邮箱，遇到未访问的邮箱，开启一次dfs搜集连通块
        for(auto& e : allEmail)
        {
            if(!vis.count(e))
            {
                vector<string> temp;
                dfs(e, temp);
                sort(temp.begin(), temp.end());  // 邮箱排序
                temp.insert(temp.begin(), emailToName[e]); // 放名字
                res.push_back(temp);
            }
        }
        return res;
    }
};
// @lc code=end

