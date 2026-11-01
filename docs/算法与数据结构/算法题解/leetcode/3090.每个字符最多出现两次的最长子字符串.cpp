#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

/*
 * @lc app=leetcode.cn id=3090 lang=cpp
 *
 * [3090] 每个字符最多出现两次的最长子字符串
 */

 // @lc code=start
class Solution {
public:
    int maximumLengthSubstring(string s) {
        int res = 0;
        unordered_map<char, int> mp;
        int left = 0;
        for (int i = 0; i < s.size(); i++) {
            mp[s[i]]++;    // 统计数据
            while (mp[s[i]] > 2) {  // 左边界右移直到符合条件
                mp[s[left]]--;
                left++;
            }
            res = max(i - left + 1, res);
        }
        return res;
    }
};
// @lc code=end

int main() {
    Solution sol;
    string s;
    cin >> s;
    int res = sol.maximumLengthSubstring(s);
    cout << s;
    return 0;
}