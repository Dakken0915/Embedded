/*
 * @lc app=leetcode.cn id=2171 lang=cpp
 *
 * [2171] 拿出最少数目的魔法豆
 */

// @lc code=start
class Solution {
public:
    long long minimumRemoval(vector<int>& beans) {
        sort(beans.begin(), beans.end());

        long long sum = 0;
        int n = beans.size();
        long long maxsave = 0;

        for(int i = 0; i < n; i++) {
            sum += beans[i];
            maxsave = max(maxsave, 1LL * beans[i] * (n - i));
        }
        return sum - maxsave;
    }
};
// @lc code=end

