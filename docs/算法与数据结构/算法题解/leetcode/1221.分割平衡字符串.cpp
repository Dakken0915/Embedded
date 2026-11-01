/*
 * @lc app=leetcode.cn id=1221 lang=cpp
 *
 * [1221] 分割平衡字符串
 */

// @lc code=start
class Solution {
public:
    int balancedStringSplit(string s) {
        int res = 0;
        int n = s.size();
        int countR = 0;
        int countL = 0;
        for(int i = 0; i < n; i++) {
            if(s[i] == 'R') countR++;
            else countL++;
            if(countR == countL) {
                countR = 0;
                countL = 0;
                res++;
            }
        }
        return res;
    }
};
// @lc code=end

