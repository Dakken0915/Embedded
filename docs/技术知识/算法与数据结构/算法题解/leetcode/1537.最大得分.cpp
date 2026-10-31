/*
 * @lc app=leetcode.cn id=1537 lang=cpp
 *
 * [1537] 最大得分
 */

// @lc code=start
class Solution {
public:
    int maxSum(vector<int>& nums1, vector<int>& nums2) {
        // 遇到岔口就更新二者的最大值
        int MOD = 1e9 + 7;
        long long sum1 = 0;
        long long sum2 = 0;
        int i = 0;
        int j = 0;
        while(j < nums2.size() && i < nums1.size()) {
            if(nums1[i] == nums2[j]) {
                sum1 = sum2 = (max(sum1, sum2) + nums1[i++]) % MOD;
                j++;
            }
            else if(nums1[i] < nums2[j])
                sum1 += nums1[i++];
            else
                sum2 += nums2[j++];
        }
        // 处理剩下没走完的元素
        while(i < nums1.size()) sum1 += nums1[i++];
        while(j < nums2.size()) sum2 += nums2[j++];
        return (int)(max(sum1, sum2) % MOD);
    }
};
// @lc code=end

