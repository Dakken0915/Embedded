/*
 * @lc app=leetcode.cn id=3795 lang=cpp
 *
 * [3795] 不同元素和至少为 K 的最短子数组长度
 */

// @lc code=start
#include <climits>
class Solution {
public:
    int minLength(vector<int>& nums, int k) {
        // cnt[val]：当前滑动窗口内数值 val 的出现次数
        unordered_map<int, int> cnt;
        int n = nums.size();
        int res = INT_MAX;
        int left = 0;
        // distinct_sum：窗口内【去重后所有不同数字的累加和】
        long long distinct_sum = 0;

        for(int i = 0; i < n; i++)
        {
            int val = nums[i];
            // 当前数字第一次进入窗口：加入去重总和
            if(cnt[val] == 0)
                distinct_sum += val;
            cnt[val]++;

            // 一旦去重和 >=k，持续收紧左边界，尝试寻找更短合法区间
            while(distinct_sum >= k)
            {
                // 更新最短长度
                res = min(res, i - left + 1);

                int l_val = nums[left];
                // 如果左端点数字在窗口内只出现一次，移除后总和要减掉该值
                if(cnt[l_val] == 1)
                    distinct_sum -= l_val;

                cnt[l_val]--;
                left++;
            }
        }
        return res == INT_MAX ? -1 : res;
    }
};
// @lc code=end

