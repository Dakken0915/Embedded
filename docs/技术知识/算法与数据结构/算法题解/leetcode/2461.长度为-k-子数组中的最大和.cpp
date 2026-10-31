/*
 * @lc app=leetcode.cn id=2461 lang=cpp
 *
 * [2461] 长度为 K 子数组中的最大和
 */

// @lc code=start
class Solution {
public:
    long long maximumSubarraySum(vector<int>& nums, int k) {
        int left = 0;
        unordered_set<int> set;   // 统计窗口中每个数的数量
        long long res = 0;
        long long sum = 0;
        for(int i = 0; i < nums.size(); i++) {
            // 窗口存在重复数字，收缩左边界直到无重复
            while(set.count(nums[i])) {
                set.erase(nums[left]);
                sum -= nums[left];
                left++;
            }
            set.insert(nums[i]);
            sum += nums[i];
            
            while(i - left >= k) {   // 左边界右移
                set.erase(nums[left]);
                sum -= nums[left];
                left++;
            }
            if(i - left + 1 == k) res = max(res, sum);
        }
        return res;
    }
};
// @lc code=end

