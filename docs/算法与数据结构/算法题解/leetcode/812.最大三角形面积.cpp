/*
 * @lc app=leetcode.cn id=812 lang=cpp
 *
 * [812] 最大三角形面积
 */

// @lc code=start
class Solution {
public:
    double largestTriangleArea(vector<vector<int>>& points) {
        double max_area = 0.0;
        int n = points.size();
        for(int i = 0; i < n; i++){
            int x1 = points[i][0], y1 = points[i][1];
            for(int j = i+1; j < n; j++){
                int x2 = points[j][0], y2 = points[j][1];
                for(int k = j+1; k < n; k++){
                    int x3 = points[k][0], y3 = points[k][1];
                    // 叉乘算平行四边形面积
                    long long cross = (long long)(x2 - x1)*(y3 - y1) - (long long)(y2 - y1)*(x3 - x1);
                    double area = fabs(cross) / 2.0;
                    max_area = max(max_area, area);
                }
            }
        }
        return max_area;
    }
};
// @lc code=end

