/*
 * @lc app=leetcode.cn id=1423 lang=cpp
 *
 * [1423] 可获得的最大点数
 */

// @lc code=start
class Solution {
public:
    int maxScore(vector<int>& cardPoints, int k) {
        // 不拿的是中间 n - k 个连续数组
        int left = 0;
        int sum = 0;
        int minpart = INT_MAX;  // 中间数组的最小值
        int temp = 0;
        for(int i = 0 ; i < cardPoints.size(); i++) {
            sum += cardPoints[i];
            temp += cardPoints[i];
            while(i - left + 1 > cardPoints.size() - k) {
                temp -= cardPoints[left];
                left++;
            }
            if(i - left + 1 == cardPoints.size() - k)
                minpart = min(minpart, temp);
        }
        return sum - minpart;
    }
};
// @lc code=end

