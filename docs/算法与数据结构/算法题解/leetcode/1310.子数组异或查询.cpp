/*
 * @lc app=leetcode.cn id=1310 lang=cpp
 *
 * [1310] 子数组异或查询
 */

// @lc code=start
class Solution {
public:
    vector<int> xorQueries(vector<int>& arr, vector<vector<int>>& queries) {
        vector<int> res;
        vector<int> prev(arr.size() + 1, 0);
        for(int i = 0; i < arr.size(); i++)
            prev[i + 1] = prev[i] ^ arr[i];

        for(int i = 0; i < queries.size(); i++) {
            int x = prev[queries[i][1] + 1] ^ prev[queries[i][0]];
            res.push_back(x);
        }
        return res;
    }
};
// @lc code=end

