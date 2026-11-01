/*
 * @lc app=leetcode.cn id=3170 lang=cpp
 *
 * [3170] 删除星号以后字典序最小的字符串
 */

// @lc code=start
class Solution {
public:
    string clearStars(string s) {
        vector<vector<int>> stack(26);
        for(int i = 0; i < s.size(); i++) {
            if(s[i] != '*') {
                stack[s[i] - 'a'].push_back(i);
                continue;
            }
            // 遇到*，从小到大找存在的字母，删掉最近一个
            for(auto& st : stack) {
                if(!st.empty()) {
                    st.pop_back();
                    break;
                }
            }
        }
        vector<int> idx;
        for(auto& st : stack)
            for(int index : st)
                idx.push_back(index);
        // 对索引进行排序
        sort(idx.begin(), idx.end());

        string res;
        for(int i : idx)
            res.push_back(s[i]);
        return res;
    }
};
// @lc code=end

