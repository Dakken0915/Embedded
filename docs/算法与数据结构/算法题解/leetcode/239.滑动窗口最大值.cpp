/*
 * @lc app=leetcode.cn id=239 lang=cpp
 *
 * [239] 滑动窗口最大值
 */

// @lc code=start
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> res;
        int left = 0;
        int n = nums.size();
        deque<int> dq;    // 存下标，维护递减

        for(int i = 0 ; i < n; i++) {
            // 窗口范围 [i-k+1, i]，如果队首下标 < i-k+1，说明已离开窗口
            if (!dq.empty() && dq.front() < i - k + 1)
                dq.pop_front();

            //维护单调递减：队尾所有小于 nums[i] 的弹出
            while (!dq.empty() && nums[dq.back()] <= nums[i])
                dq.pop_back();
            
            // 加入当前元素
            dq.push_back(i);
            
            // 记录窗口最大值
            if (i >= k - 1)
                res.push_back(nums[dq.front()]);
        }
        return res;
    }
};
// @lc code=end

