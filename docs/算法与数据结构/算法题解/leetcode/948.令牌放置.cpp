/*
 * @lc app=leetcode.cn id=948 lang=cpp
 *
 * [948] 令牌放置
 */

// @lc code=start
class Solution {
public:
    int bagOfTokensScore(vector<int>& tokens, int power) {
        sort(tokens.begin(), tokens.end());
        int score = 0;
        int res = 0;
        int left = 0;
        int right = tokens.size() - 1;
        while(left <= right) {
            // 优先换取令牌值较小的分数
            if(power >= tokens[left]) {
                power -= tokens[left];
                score++;
                res = max(res, score);
                left++;
            }
            // 用分数换取令牌值较大的 power
            else if(score) {
                score--;
                power += tokens[right];
                right--;
            }
            else  // 换不了 power 也得不到分数
                break;
        }
        return res;
    }
};
// @lc code=end

