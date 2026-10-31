/*
 * @lc app=leetcode.cn id=2144 lang=cpp
 *
 * [2144] 打折购买糖果的最小开销
 */

// @lc code=start
class Solution {
public:
    int minimumCost(vector<int>& cost) {
        priority_queue<int> maxHeap;
        for(int i = 0; i < cost.size(); i++)
            maxHeap.push(cost[i]);

        int count = 0; // 用来计数
        int res = 0;
        while(!maxHeap.empty()) {
            res += maxHeap.top();
            maxHeap.pop();
            count++;
            if(count >= 2) {
                if(!maxHeap.empty())  // 这里注意防空判断
                    maxHeap.pop();
                count = 0;
            }
        }
        return res;
    }
};
// @lc code=end

