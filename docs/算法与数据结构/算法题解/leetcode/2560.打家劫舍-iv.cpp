/*
 * @lc app=leetcode.cn id=2560 lang=cpp
 *
 * [2560] 打家劫舍 IV
 */

// @lc code=start
class Solution {
private:
    bool check(vector<int>& nums, int k, int mx) {
        // f0 保存上一个房屋可以偷的数量
        int f0 = 0, f1 = 0;  // 滚动变量，模拟DP
        for(int x : nums) {
            if(x > mx) // 不可以偷
                f0 = f1;
            else {
                int tmp = f1; // 保存旧f1（下一轮的f0）
                // 「前前一间的最优数量」+ 当前这间 或 前一间的最优解
                f1 = max(f1, f0 + 1);
                f0 = tmp;
            }
        }
        return f1 >= k;
    }
public:
    // 给定数组 nums，从中选择一个长度至少为 k 的子序列 A，
    // 要求 A 中没有任何元素在 nums 中是相邻的。最小化 max(A)。
    int minCapability(vector<int>& nums, int k) {
        int left = 0;
        int right = *max_element(nums.begin(), nums.end());
        while(left + 1 < right) {  // 开区间写法
            int mid = left + (right - left) / 2;
            // 可以达到指定数量，将 mid 向下压缩
            (check(nums, k, mid) ? right : left) = mid;
        }
        return right;
    }
};
// @lc code=end

