/*
 * @lc app=leetcode.cn id=1441 lang=cpp
 *
 * [1441] 用栈操作构建数组
 */

// @lc code=start
class Solution {
public:
    vector<string> buildArray(vector<int>& target, int n) {
        vector<string> str;
        int index = 0;
        for(int i = 1; i <= n; i++) {
            if(i == target[index]) {
                str.push_back("Push");
                index++;
            }
            else {
                str.push_back("Push");
                str.push_back("Pop");
            }
            if(index == target.size()) break;
        }
        return str;
    }
};
// @lc code=end

