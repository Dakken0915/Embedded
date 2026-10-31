/*
 * @lc app=leetcode.cn id=1283 lang=cpp
 *
 * [1283] 使结果不超过阈值的最小除数
 */

// @lc code=start
class Solution {
public:
    // 二分答案找最小值
    int smallestDivisor(vector<int>& nums, int threshold) {
        int right = INT_MIN;
        for(int x : nums)
            right = max(right, x);
        int left = 0; 

        while(left + 1 < right) {
            int mid = left + (right - left) / 2;
            int n = mid - 1;
            int sum = 0;

            // check 过程
            for(int x : nums)
                sum += (x + n) / mid;
            
            if(sum <= threshold) // 满足条件
                right = mid;
            else   // 不满足条件
                left = mid;
        }
        return right;
    }
};
// @lc code=end

