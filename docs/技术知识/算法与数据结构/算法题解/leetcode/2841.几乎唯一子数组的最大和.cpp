/*
 * @lc app=leetcode.cn id=2841 lang=cpp
 *
 * [2841] 几乎唯一子数组的最大和
 */

// @lc code=start
class Solution {
public:
    long long maxSum(vector<int>& nums, int m, int k) {
        if(!nums.size()) return 0;
        unordered_map<int, int> mp;
        long long res = 0;
        long long temp = 0; // 保存唯一子数组和
        int left = 0;
        int count = 0; // 可重复的个数
        for(int i = 0; i < nums.size(); i++) {
            if(mp[nums[i]] > 0) { // 记录重复部分
                temp += nums[i];
                count++;
            }
            else temp += nums[i];
            mp[nums[i]]++;

            // 处理超出部分
            while(count > k - m) {   
                if(mp[nums[left]] > 1)
                    count--;  
                mp[nums[left]]--;
                temp -= nums[left];
                left++;
            }
            
            // 移动左边界
            while(i - left + 1 > k) {
                if(mp[nums[left]] > 1)
                    count--;
                mp[nums[left]]--;
                temp -= nums[left];
                left++;
            }
            if(i - left + 1 == k)
                res = max(res, temp);
        }
        return res;
    }
};
// @lc code=end

