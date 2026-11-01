/*
 * @lc app=leetcode.cn id=1658 lang=cpp
 *
 * [1658] 将 x 减到 0 的最小操作数
 */

// @lc code=start
class Solution {
public:
    int minOperations(vector<int>& nums, int x) {
        // 符合条件：中间连续子数组和等于 sum - x
        int sum = reduce(nums.begin(), nums.end());
        if(sum < x) return -1;
        int n = nums.size();
        int res = -1;

        int left = 0;
        int partSum = 0;

        for(int i = 0; i < n; i++) {
            partSum += nums[i];

            // 不符合条件时收缩窗口
            while(partSum > sum - x) {
                partSum -= nums[left];
                left++;
            }
            // 符合要求，更新答案
            if(partSum == sum - x) res = max(res, i - left + 1);
        }
        if(res == -1) return -1;
        return n - res;
    }
};
// @lc code=end

// 递归：大数据必定超时
class Solution {
private:
    // x 为目标值，cnt 为操作计数值
    int dfs(vector<int>& nums, int x, int cnt, int left, int right) {
        if(left > right) return INT_MAX;
        if(nums[left] > x && nums[right] > x) return INT_MAX;
        if(nums[left] == x || nums[right] == x) return cnt + 1;

        // 选择左边
        int a = dfs(nums, x - nums[left], cnt + 1, left + 1, right);
        // 选择右边
        int b = dfs(nums, x - nums[right], cnt + 1, left, right - 1);
        return min(a, b);
    }

public:
    int minOperations(vector<int>& nums, int x) {
        int res = dfs(nums, x, 0, 0, nums.size() - 1);
        return res == INT_MAX ? -1 : res;
    }
};