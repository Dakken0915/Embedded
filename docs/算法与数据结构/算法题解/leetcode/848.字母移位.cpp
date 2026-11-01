/*
 * @lc app=leetcode.cn id=848 lang=cpp
 *
 * [848] 字母移位
 */

// @lc code=start
class Solution {
public:
    string shiftingLetters(string s, vector<int>& shifts) {
        vector<int> prev(shifts.size() + 1, 0);
        for(int i = shifts.size() - 1; i >= 0; i--)
            prev[i] = (prev[i + 1] + shifts[i]) % 26;

        for(int i = 0; i < shifts.size(); i++)
            s[i] = (s[i] - 'a' + prev[i]) % 26 + 'a';
        return s;
    }
};
// @lc code=end

