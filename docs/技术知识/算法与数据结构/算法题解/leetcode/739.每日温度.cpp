/*
 * @lc app=leetcode.cn id=739 lang=cpp
 *
 * [739] 每日温度
 */

// @lc code=start
class Solution {
public:
    // 单调栈
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        int n = temperatures.size();
        vector<int> res(n, 0);
        
        stack<int> st;  // 单调栈 -> 存下标

        for(int i = 0; i < n; i++) {
            int val = temperatures[i];

            // 当前值大于栈顶元素，更新栈顶元素下标答案
            while(!st.empty() && val > temperatures[st.top()]) {
                int index = st.top();
                res[index] = i - index;
                st.pop();  // 获取到答案，出栈
            }

            st.push(i);
        }
        return res;
    }
};
// @lc code=end

// 暴力搜索，超时
class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        int n = temperatures.size();

        vector<int> res(n, 0);
        for(int i = 0; i < n; i++) {
            int base = temperatures[i];
            for(int j = i + 1; j < n; j++) {
                if(temperatures[j] > base) {
                    res[i] = j - i;
                    break;
                }
            }
        }
        return res;
    }
};