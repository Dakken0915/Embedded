/*
 * @lc app=leetcode.cn id=3794 lang=cpp
 *
 * [3794] 反转字符串前缀
 */

// @lc code=start
class Solution {
public:
    string reversePrefix(string s, int k) {
        int left = 0;
        int right = k - 1;
        while(left < right) {
            swap(s[left], s[right]);
            left++;
            right--;
        }
        return s;
    }
};
// @lc code=end

