/*
 * @lc app=leetcode.cn id=2379 lang=cpp
 *
 * [2379] 得到 K 个黑块的最少涂色次数
 */

// @lc code=start
class Solution {
public:
    int minimumRecolors(string blocks, int k) {
        int left = 0;
        int n = blocks.size();

        int res = INT_MAX;
        int cnt = 0;
        for(int right = 0; right < n; right++) {
            if(blocks[right] == 'W')
                cnt++;

            if(right >= k - 1) {
                res = min(res, cnt);
                if(blocks[left] == 'W')
                    cnt--;
                left++;
            }
        }
        return res;
    }
};
// @lc code=end

