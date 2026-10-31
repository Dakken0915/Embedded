/*
 * @lc app=leetcode.cn id=2116 lang=cpp
 *
 * [2116] 判断一个括号字符串是否有效
 */

// @lc code=start
class Solution {
public:
    bool canBeValid(string s, string locked) {
        if (s.size() % 2) return false;

        // mx：最大可能平衡值  mn：最小
        int mn = 0, mx = 0;
        for (int i = 0; i < s.size(); i++) {
            if (locked[i] == '1') { // 不能改
                int d = s[i] == '(' ? 1 : -1;
                mx += d;
                if (mx < 0) { // c 不能为负
                    return false;
                }
                mn += d;
            } else { // 可以改
                mx++; // 改成左括号，c 加一
                mn--; // 改成右括号，c 减一
            }
            if (mn < 0) { // c 不能为负
                mn = 1; // 此时 c 的取值范围都是奇数，最小的正奇数是 1
            }
        }
        return mn == 0; // 说明最终 c 能是 0
    }
};
// @lc code=end

