/*
 * @lc app=leetcode.cn id=904 lang=cpp
 *
 * [904] 水果成篮
 */

// @lc code=start
class Solution {
public:
    int totalFruit(vector<int>& fruits) {
        int n = fruits.size();
        int res = INT_MIN;

        int left = 0;
        unordered_map<int, int> map;
        for(int i = 0; i < n; i++) {
            map[fruits[i]]++;
            
            while(map.size() > 2) {
                map[fruits[left]]--;

                // 如果不存在该种类就将其擦除
                if(map[fruits[left]] == 0)
                    map.erase(fruits[left]);

                left++;
            }
            // 只要窗口合法就更新答案
            res = max(res, i - left + 1);
        }
        return res;
    }
};
// @lc code=end

