/*
 * @lc app=leetcode.cn id=3355 lang=cpp
 *
 * [3355] 零数组变换 I
 */

// @lc code=start
class Solution {
public:
    bool isZeroArray(vector<int>& nums, vector<vector<int>>& queries) {
        int n = nums.size();
        vector<int> diff(n + 1);
        for (auto& q : queries) {
            diff[q[0]]++;
            diff[q[1] + 1]--;
        }

        int sum_d = 0;
        for (int i = 0; i < n; i++) {
            sum_d += diff[i];
            // 此时 sum_d 表示 nums[i] 要减掉的值
            if (nums[i] > sum_d) // nums[i] 无法变成 0
                return false;
        }
        return true;
    }
};
// @lc code=end

