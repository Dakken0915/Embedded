/*
 * @lc app=leetcode.cn id=3074 lang=cpp
 *
 * [3074] 重新分装苹果
 */

// @lc code=start
#include <queue>
class Solution {
public:
    int minimumBoxes(vector<int>& apple, vector<int>& capacity) {
        priority_queue<int> maxHeap;
        int count = 0;
        int sum = 0;
        int x = 0;
        for(int i = 0; i < capacity.size(); i++)
            maxHeap.push(capacity[i]);
        for(int i = 0; i < apple.size(); i++)
            sum += apple[i];
        while(x < sum && !maxHeap.empty()) {
            x += maxHeap.top();
            maxHeap.pop();
            count++;
        }
        return count;
    }
};
// @lc code=end

