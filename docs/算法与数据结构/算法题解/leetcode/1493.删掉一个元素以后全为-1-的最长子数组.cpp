#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
 * @lc app=leetcode.cn id=1493 lang=cpp
 *
 * [1493] 删掉一个元素以后全为 1 的最长子数组
 */

 // @lc code=start
class Solution {
public:
    int longestSubarray(vector<int>& nums) {
        vector<int> zeroIdx;
        int n = nums.size();
        for (int i = 0; i < n; i++) {
            if (nums[i] == 0)
                zeroIdx.push_back(i);
        }

        // 无0，必须删一个1
        if (zeroIdx.empty())
            return n - 1;

        int maxLen = 0;
        int m = zeroIdx.size();

        // 遍历每个0作为被删除的元素
        for (int i = 0; i < m; i++) {
            int left;
            if (i == 0) left = zeroIdx[i]; // 第一个0左侧全部1
            else left = zeroIdx[i] - zeroIdx[i-1] - 1;

            int right;
            if (i == m - 1) right = n - zeroIdx[i] - 1; // 最后一个0右侧全部1
            else right = zeroIdx[i+1] - zeroIdx[i] - 1;

            // 删除当前0，左右1合并
            maxLen = max(maxLen, left + right);
        }
        return maxLen;
    }
};
// @lc code=end

int main() {
    vector<int> nums;
    int x;
    while (cin >> x)
        nums.push_back(x);
    int res = longestSubarray(nums);
    cout << res;
    return 0;
}
