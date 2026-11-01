/*
 * @lc app=leetcode.cn id=3453 lang=cpp
 *
 * [3453] 分割正方形 I
 */

// @lc code=start
class Solution {
public:
    double separateSquares(vector<vector<int>>& squares) {
        long long total_area = 0;
        map<int, long long> diff;
        for(auto& x : squares) {
            int y = x[1];
            int l = x[2];
            total_area += 1LL * l * l; // 累加总面积
            diff[y] += l; // 记录有效边长
            diff[y + l] -= l;
        }

        long long sum_len = 0;
        long long area = 0;
        // 从小到大遍历所有 y 临界点
        auto it = diff.begin();
        while(next(it) != diff.end()){
            auto [y, delta] = *it;
            // 取出下一个 y 临界点
            int y_next = next(it)->first;

            sum_len += delta;
            long long seg = sum_len * (y_next - y);

            // 小于总面积的一半
            if(area + seg < total_area / 2.0){
                area += seg;
                it++;
            }else{
                double need = total_area / 2.0 - area;
                return y + need / sum_len;
            }
        }
        return 0.0; // 理论不会走到这里
    }
};
// @lc code=end

