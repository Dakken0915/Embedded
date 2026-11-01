/*
 * @lc app=leetcode.cn id=2405 lang=cpp
 *
 * [2405] 子字符串的最优划分
 */

// @lc code=start
class Solution {
public:
    int partitionString(string s) {
        int res = 0;
        unordered_set<char> set;
        for(char ch : s) {
            if(set.count(ch)) {
                set.clear();
                res++;
                set.insert(ch);
            }
            else
                set.insert(ch);
        }
        return res + 1;
    }
};
// @lc code=end

