// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem213.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=213 lang=cpp
 *
 * [213] 打家劫舍 II
 */

// @lc code=start
class Solution {
public:
    int rob1(vector<int>& nums, int start, int end) {
        int f0 = 0, f1 = 0;
        for (int i = start; i < end; i++) {
            int new_f = max(f1, f0 + nums[i]);
            f0 = f1;
            f1 = new_f;
        }
        return f1;
    }
    
    int rob(vector<int>& nums) {
        // 选择偷第一间或者不偷
        int n = nums.size();
        return max(nums[0] + rob1(nums, 2, n - 1), rob1(nums, 1, n));
    }
};
// @lc code=end

