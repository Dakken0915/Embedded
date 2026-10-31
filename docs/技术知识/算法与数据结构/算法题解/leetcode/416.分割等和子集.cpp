/*
 * @lc app=leetcode.cn id=416 lang=cpp
 *
 * [416] 分割等和子集
 */

// @lc code=start

// 记忆化递归优化时间
class Solution {
private:
    vector<vector<int>> memo;
    bool dfs(vector<int>& nums, int i, int target) {
        if(i < 0) return target == 0;
        if(memo[i][target] != -1) return memo[i][target] == 1;

        bool res;
        if(nums[i] > target)
            res = dfs(nums, i - 1, target);
        else
            res = dfs(nums, i - 1, target - nums[i]) || dfs(nums, i - 1, target);

        memo[i][target] = res ? 1 : 0;
        return res;
    }
public:
    bool canPartition(vector<int>& nums) {
        int n = nums.size();
        int sum = reduce(nums.begin(), nums.end());
        int target = sum / 2;
        if(sum % 2 != 0) return false;
        memo.assign(n, vector<int>(target + 1, -1));
        return  dfs(nums, n - 1, target);
    }
};
// @lc code=end

