/*
 * @lc app=leetcode.cn id=42 lang=cpp
 *
 * [42] 接雨水
 */

// @lc code=start
class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        int left = 0;
        int right = n - 1;
        int res = 0;
        int maxL = 0;   // 左侧最高高度
        int maxR = 0;   // 右侧最高高度
        // 这里注意不要漏掉中间的柱子
        while(left <= right) {
            // 优先移动最高高度较小的一侧
            if(maxL < maxR) {
                // 无法蓄水
                if(height[left] >= maxL)
                    maxL = height[left];
                else // 可以蓄水
                    res += maxL - height[left];
                left++;
            }
            else {
                // 无法蓄水
                if(height[right] >= maxR)
                    maxR = height[right];
                else // 可以蓄水
                    res += maxR - height[right];
                right--;
            }
        }
        return res;
    }
};
// @lc code=end

