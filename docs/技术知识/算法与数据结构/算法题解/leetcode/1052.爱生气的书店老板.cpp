/*
 * @lc app=leetcode.cn id=1052 lang=cpp
 *
 * [1052] 爱生气的书店老板
 */

// @lc code=start
class Solution {
public:
    int maxSatisfied(vector<int>& customers, vector<int>& grumpy, int minutes) {
        int n = customers.size();
        int res = INT_MIN;
        int cnt = 0;
        int left = 0;

        for(int i = 0; i < n; i++)
            if(grumpy[i] == 0)
                cnt += customers[i];

        for(int i = 0; i < n; i++) {
            if(grumpy[i] == 1)
                cnt += customers[i];

            if(i >= minutes - 1) {
                res = max(res, cnt);
                if(grumpy[left] == 1)
                    cnt -= customers[left];
                left++;
            }
        }
        return res;
    }
};
// @lc code=end

