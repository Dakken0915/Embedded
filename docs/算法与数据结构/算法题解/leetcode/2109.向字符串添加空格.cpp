/*
 * @lc app=leetcode.cn id=2109 lang=cpp
 *
 * [2109] 向字符串添加空格
 */

// @lc code=start
class Solution {
public:
    string addSpaces(string s, vector<int>& spaces) {
        string res;
        int i = 0;
        int j = 0;
        while(i < s.size()) {
            // 当前位置需要插入空格
            if(j < spaces.size() && i == spaces[j]) {
                res += ' ';
                j++;
            }
            else {
                res += s[i];
                i++;
            }
        }
        return res;
    }
};
// @lc code=end

