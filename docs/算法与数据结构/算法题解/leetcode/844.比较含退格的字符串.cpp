/*
 * @lc app=leetcode.cn id=844 lang=cpp
 *
 * [844] 比较含退格的字符串
 */

// @lc code=start
class Solution {
public:
    bool backspaceCompare(string s, string t) {
        int i = s.size() - 1;
        int j = t.size() - 1;
        int skipS = 0, skipT = 0;

        while(i >= 0 || j >= 0) {
            // 寻找s有效字符
            while (i >= 0) {
                if (s[i] == '#') {
                    skipS++; i--;
                } else if (skipS > 0) {
                    skipS--; i--;
                } else break;
            }
            // 寻找t有效字符
            while (j >= 0) {
                if (t[j] == '#') {
                    skipT++; j--;
                } else if (skipT > 0) {
                    skipT--; j--;
                } else break;
            }
            // 只有一个有效
            if(i >= 0 ^ j >= 0)
                return false;
            // 两边都存在有效字符
            if(i >= 0 && j >= 0 && s[i] != t[j])
                    return false;
            i--;
            j--;
        }
        return true;
    }
};
// @lc code=end

