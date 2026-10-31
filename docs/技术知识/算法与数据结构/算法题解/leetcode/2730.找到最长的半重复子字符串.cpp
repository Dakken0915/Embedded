/*
 * @lc app=leetcode.cn id=2730 lang=cpp
 *
 * [2730] 找到最长的半重复子字符串
 */

// @lc code=start
class Solution {
public:
    int longestSemiRepetitiveSubstring(string s) {
        if(s.empty()) return 0;
        int n = s.size();
        int res = 1;   // 至少有1项

        int left = 0;
        int cnt = 0; // 统计窗口内相邻重复数字的对数
        for(int i = 0; i < n; i++) {
            if(!i) continue;  // 跳过第一项
            if(s[i] == s[i - 1])
                cnt++;
            
            // 不符合条件时收缩窗口
            while(cnt > 1) {
                if(s[left] == s[left + 1])
                    cnt--;
                left++;
            }
            // 符合条件时更新答案
            res = max(res, i - left + 1);
        }
        return res;
    }
};
// @lc code=end

