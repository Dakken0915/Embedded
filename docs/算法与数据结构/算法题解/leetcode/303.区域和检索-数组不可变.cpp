/*
 * @lc app=leetcode.cn id=303 lang=cpp
 *
 * [303] 区域和检索 - 数组不可变
 */

// @lc code=start
class NumArray {
    vector<int> s;

public:
    NumArray(vector<int>& nums) {
        s.assign(nums.size() + 1, 0);
        for(int i = 0; i < nums.size(); i++)
            s[i + 1] = s[i] + nums[i];
    }
    
    int sumRange(int left, int right) {
        return s[right + 1] - s[left];
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * int param_1 = obj->sumRange(left,right);
 */
// @lc code=end

