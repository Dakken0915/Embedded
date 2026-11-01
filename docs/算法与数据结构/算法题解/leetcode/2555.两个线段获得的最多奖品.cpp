/*
 * @lc app=leetcode.cn id=2555 lang=cpp
 *
 * [2555] 两个线段获得的最多奖品
 */

// @lc code=start
class Solution {
public:
    int maximizeWin(vector<int>& prizePositions, int k) {
        int n = prizePositions.size();
        if(n == 0) return 0;

        // suffix[i] 表示从 i 到 n-1 覆盖的奖品数
        vector<int> cover(n + 1, 0);
        int right = 0;
        for(int left = 0; left < n; left++) {
            // 移动右指针
            while(right < n && prizePositions[right] - prizePositions[left] <=k)
                right++;
            
            cover[left] = right - left; // 统计覆盖的奖品数量
        }

        // 从后往前计算后缀最大值
        vector<int> suffix(n + 1, 0);
        for(int i = n - 1; i >= 0; i--) {
            suffix[i] = max(suffix[i + 1], cover[i]);
        }

        int ans = 0;
        right = 0;
        for(int left = 0; left < n; left++) {
            while(right < n && prizePositions[right] - prizePositions[left] <=k)
                right++;
                
            // 第二条线段从 right 开始
            ans = max(ans, cover[left] + suffix[right]);
        }
        return ans;
    }
};
// @lc code=end

