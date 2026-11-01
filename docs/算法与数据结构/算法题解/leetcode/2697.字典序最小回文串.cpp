/*
 * @lc app=leetcode.cn id=2697 lang=cpp
 *
 * [2697] 字典序最小回文串
 */

// @lc code=start
class Solution {
public:
    string makeSmallestPalindrome(string s) {
        int left = 0;
        int right = s.size() - 1;
        while(left < right) {
            if(s[left] == s[right]) continue;
            // 字符不同的处理
            s[left] = s[left] < s[right] ? s[left] : s[right];
            s[right] = s[left];
            left++;
            right--;
        }
        return s;
    }
};
// @lc code=end

