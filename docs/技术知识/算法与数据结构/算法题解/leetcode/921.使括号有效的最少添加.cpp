/*
 * @lc app=leetcode.cn id=921 lang=cpp
 *
 * [921] 使括号有效的最少添加
 */

// @lc code=start
class Solution {
public:
    int minAddToMakeValid(string s) {
        int count = 0; 
        int res = 0;
        for(char c : s) {
            if(c == '(')
                count++;
            else if(c == ')' && count == 0)
                res++;
            else 
                count--;
        }
        return res + count;
    }
};
// @lc code=end

