/*
 * @lc app=leetcode.cn id=682 lang=cpp
 *
 * [682] 棒球比赛
 */

// @lc code=start
class Solution {
public:
    int calPoints(vector<string>& operations) {
        stack<int> st;
        int res = 0;
        for(string s : operations) {
            if(s == "C") {
                res -= st.top();
                st.pop();
            }
            else if(s == "D") {
                res += st.top() * 2;
                st.push(st.top() * 2);
            }
            else if(s == "+") {
                int a = st.top();
                st.pop();
                int sum = a + st.top();
                res += sum;
                st.push(a);
                st.push(sum);
            }
            else {
                res += stoi(s);
                st.push(stoi(s));
            }
        }
        return res;
    }
};
// @lc code=end

