/*
 * @lc app=leetcode.cn id=1475 lang=cpp
 *
 * [1475] 商品折扣后的最终价格
 */

// @lc code=start
class Solution {
public:
    vector<int> finalPrices(vector<int>& prices) {
        int n = prices.size();
        vector<int> res(n);

        stack<int> st;

        for(int i = 0; i < n; i++) {
            res[i] = prices[i];
            while(!st.empty() && prices[i] <= prices[st.top()]) {
                res[st.top()] = prices[st.top()] - prices[i];
                st.pop();
            }
            st.push(i);
        }
        return res;
    }
};
// @lc code=end

