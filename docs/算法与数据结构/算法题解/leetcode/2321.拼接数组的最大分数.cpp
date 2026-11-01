/*
 * @lc app=leetcode.cn id=2321 lang=cpp
 *
 * [2321] 拼接数组的最大分数
 */

// @lc code=start
class Solution {
public:
    int maximumsSplicedArray(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size();
        int sum1 = reduce(nums1.begin(), nums1.end());
        int sum2 = reduce(nums2.begin(), nums2.end());
        vector<int> diff(n);
        for(int i = 0; i < n; i++)
            diff[i] = nums1[i] - nums2[i];
        
        // kadane 算法
        int prevMin = 0, prevMax = 0;
        int minArea = INT_MAX, maxArea = INT_MIN;
        for(int i = 0; i < n; i++) {
            // 找最小值交换给nums1
            int curMin = min(prevMin, 0) + diff[i];
            prevMin = curMin;
            minArea = min(minArea, curMin);

            // 找最大值交换给nums2
            int curMax = max(prevMax, 0) + diff[i];
            prevMax = curMax;
            maxArea = max(maxArea, curMax);
        }

        int change = max(sum1-minArea, sum2+maxArea);
        int nochange = max(sum1, sum2);

        return max(change, nochange);
    }
};
// @lc code=end

