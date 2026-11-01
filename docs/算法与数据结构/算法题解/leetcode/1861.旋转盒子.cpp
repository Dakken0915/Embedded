/*
 * @lc app=leetcode.cn id=1861 lang=cpp
 *
 * [1861] 旋转盒子
 */

// @lc code=start
class Solution {
public:
    vector<vector<char>> rotateTheBox(vector<vector<char>>& boxGrid) {
        int m = boxGrid.size();
        int n = boxGrid[0].size();
        vector<vector<char>> res(n);

        // 逐行处理
        for(int i = 0; i < m; i++) {
            /* 思路：right 指针表示该段可以向右沉的最低位置的下一个
            left 指针从右往左扫描，根据扫描到的物品进行处理 */
            int right = n;
            int left = n - 1;
            while(left >= 0) {  // 扫到等于 0 为止
                if(boxGrid[i][left] == '*') {
                    right = left;
                    left--;
                }
                else if(boxGrid[i][left] == '#') {
                    boxGrid[i][right - 1] = '#';
                    if(left != right - 1)
                        boxGrid[i][left] = '.';  // 置空
                    right--;
                    left--;
                }
                else left--;
            }            
        }
        // 拷贝数组
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++)
                res[i].push_back(boxGrid[m - j - 1][i]);
        }
        return res;
    }
};
// @lc code=end

