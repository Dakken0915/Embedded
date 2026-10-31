/*
 * @lc app=leetcode.cn id=20 lang=cpp
 *
 * [20] 有效的括号
 */

// @lc code=start
class Solution {
public:
    bool isValid(string s) {
        stack<int> st;
        for (int i = 0; i < s.size(); i++) {
            if (s[i] == '(' || s[i] == '[' || s[i] == '{') st.push(i);
            else {
                if (st.empty()) return false;
                if (s[i] == ')' && s[st.top()] != '(') return false;
                if (s[i] == '}' && s[st.top()] != '{') return false;
                if (s[i] == ']' && s[st.top()] != '[') return false;
                st.pop();
            }
        }
      return st.empty();
    }
};
// @lc code=end

