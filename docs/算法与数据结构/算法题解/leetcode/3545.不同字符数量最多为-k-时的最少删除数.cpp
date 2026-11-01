/*
 * @lc app=leetcode.cn id=3545 lang=cpp
 *
 * [3545] 不同字符数量最多为 K 时的最少删除数
 */

// @lc code=start
class Solution {
public:
    int minDeletion(string s, int k) {
        vector<int> count(26, 0);
        priority_queue<int, vector<int>, greater<int>> minHeap;
        int res = 0;
        for(int i = 0; i < s.size(); i++)
            count[s[i] - 'a']++;  // 统计

        for(int i = 0; i < 26; i++)
            if(count[i] > 0)
                minHeap.push(count[i]);

        while(minHeap.size() > k) {
            res += minHeap.top();
            minHeap.pop();
        }
        return res;
    }
};
// @lc code=end

