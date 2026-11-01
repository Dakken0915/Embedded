/*
 * @lc app=leetcode.cn id=394 lang=cpp
 *
 * [394] 字符串解码
 */

// @lc code=start
class Solution {
public:
    string decodeString(string s) {
        stack<pair<int, string>> st;  // 存记录好的数字和解码好的部分字符串
        string res = "";
        int dig = 0;
        for(char ch : s) {
            if(ch == '[') {  // 压栈
                st.push({dig, res});
                dig = 0;
                res = "";
            }
            else if(ch == ']') {  // 出栈
                dig = st.top().first;
                string temp = res;  // 保存res
                res = st.top().second;
                st.pop();
                for(int i = 0; i < dig; i++)
                    res += temp;   // 重复添加字符串
                dig = 0; // 清空数字
            }
            else if(ch >= 'a' && ch <= 'z') {
                res += ch;
            }
            else if(ch >= '0' && ch <= '9') {
                dig = dig * 10 + ch - '0';
            }
        }
        return res;
    }
};
// @lc code=end

// 示例
class Solution {
public:
    string decodeString(string s) {
        // 栈：first存储重复次数，second存储'['之前已经拼接好的字符串
        stack<pair<int, string>> st;
        // res：当前括号内部正在拼接的字符串
        string res = "";
        // multi：记录数字，代表括号内字符串需要重复多少次
        int multi = 0;

        // 遍历编码字符串每一个字符
        for (char c : s) {
            if (c == '[') {
                // 遇到左括号，把当前的重复次数、括号之前已经得到的字符串压入栈保存现场
                pair<int, string> pa(multi, res);
                st.push(pa);
                // 重置，准备处理括号里面的新字符串
                res = "";
                multi = 0;
            } else if (c == ']') {
                // 遇到右括号，出栈，取出左括号时保存的上下文
                pair<int, string> p = st.top();
                st.pop();
                // 获取需要重复的次数
                multi = p.first;
                // 获取'['之前已经拼接完成的旧字符串
                string last_res = p.second;
                // 将括号内当前res重复multi次，追加到旧字符串后面
                while (multi > 0) {
                    last_res.append(res);
                    multi--;
                }
                // 更新res为拼接完成后的整体结果，继续向外层处理
                res = last_res;
            } else if (c >= 'a' && c <= 'z') {
                // 普通小写字母，直接追加到当前结果末尾
                res.push_back(c);
            } else if (c - '0' >= 0 && c - '0' <= 9) {
                // 处理多位数字：例如"12[a]"，依次读'1'、'2'得到multi=12
                multi = multi * 10 + (c - '0');
            }
        }

        // 遍历结束，res就是解码完成的字符串
        return res;
    }
};