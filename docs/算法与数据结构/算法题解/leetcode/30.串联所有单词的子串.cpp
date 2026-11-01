/*
 * @lc app=leetcode.cn id=30 lang=cpp
 *
 * [30] 串联所有单词的子串
 */

// @lc code=start
class Solution {
public:
    vector<int> findSubstring(string s, vector<string>& words) {
        int m = words.size();
        int n = words[0].size();
        vector<int> res;
        
        unordered_map<string, int> target;
        // 将每个单词记录下来
        for(string w : words)
            target[w]++;
        
        // 枚举所有偏移
        for(int offset = 0; offset < n; offset++) {
            unordered_map<string, int> window;
            int left = offset;

            for(int right = offset; right + n <= s.size(); right += n) {
                string curWord = s.substr(right, n);
                window[curWord]++;

                // 窗口总长超标：弹出最左侧单词
                while ((right - left + n) > n * m) {
                    string leftWord = s.substr(left, n);
                    window[leftWord]--;
                    if (window[leftWord] == 0) window.erase(leftWord);
                    left += n;
                }
                // 窗口频次完全匹配 → 找到答案
                if (window == target)
                    res.push_back(left);
            }
        }
        return res;
    }
};
// @lc code=end

