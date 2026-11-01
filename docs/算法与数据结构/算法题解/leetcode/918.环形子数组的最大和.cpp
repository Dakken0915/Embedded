/*
 * @lc app=leetcode.cn id=918 lang=cpp
 *
 * [918] 环形子数组的最大和
 */

// @lc code=start
class Solution {
public:
    int maxSubarraySumCircular(vector<int>& nums) {
        int n = nums.size();
        int prevMin = 0, prevMax = 0;
        int Min = 0, Max = INT_MIN;
        for(int i = 0; i < n; i++) {
            // 包含头尾，取中间最小值做差
            int curMin = min(prevMin, 0) + nums[i];
            prevMin = curMin;
            Min = min(Min, curMin);

            // 不包含头尾，直接取中间最大值
            int curMax = max(prevMax, 0) + nums[i];
            prevMax = curMax;
            Max = max(Max, curMax);
        }
        int sum = reduce(nums.begin(), nums.end());
        
        // 防止最小子数组覆盖全部元素
        if(sum - Min == 0) return Max;

        return max(sum - Min, Max);
    }
};
// @lc code=end

