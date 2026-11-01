/*
 * @lc app=leetcode.cn id=3085 lang=cpp
 *
 * [3085] 成为 K 特殊字符串需要删除的最少字符数
 */

// @lc code=start
class Solution {
public:
    int minimumDeletions(string word, int k) {
        int cnt[26]{};
        for (char c: word) {
            cnt[c - 'a']++;
        }
        ranges::sort(cnt);

        int max_save = 0;
        for (int i = 0; i < 26; i++) {
            int sum = 0;
            for (int j = i; j < 26; j++) {
                sum += min(cnt[j], cnt[i] + k); // 至多保留 cnt[i]+k 个
            }
            max_save = max(max_save, sum);
        }

        return word.size() - max_save;
    }
};
// @lc code=end

class Solution {
public:
    int minimumDeletions(string word, int k) {
        vector<int> cnt(26);
        for(char ch : word)
            cnt[ch - 'a']++;

        // 删除0元素
        auto it = remove(cnt.begin(), cnt.end(), 0);
        cnt.erase(it, cnt.end());
        
        sort(cnt.begin(), cnt.end());
        int n = cnt.size();

        int res = INT_MAX;
        vector<int> prev(n, 0);
        for(int i = 0; i < n; i++) {
            int val = cnt[i];
            // 计算前缀和
            if(i > 0)
                prev[i] = prev[i - 1] + val;
            else
                prev[i] = val;

            int temp = prev[i] - val;

            for(int j = i; j < n; j++)
                temp += max(cnt[j] - val - k, 0);
            res = min(res, temp);
        }
        return res;
    }
};