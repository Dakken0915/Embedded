#include <iostream>
#include <vector>
#include <string>
#include <sstream>

/*
 * @lc app=leetcode.cn id=643 lang=cpp
 *
 * [643] 子数组最大平均数 I
 */

// @lc code=start
class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        int n = nums.size();
        int sum = 0;  // 临时存储数组和
        // 先计算第一个窗口总和
        for(int i = 0; i < k; i++)
            sum += nums[i];
        double res = (double)sum / k;  // 解决负数用例
        for (int i = k; i < n; i++) {
            sum += nums[i];
            if (i >= k)
                sum -= nums[i - k];
            res = max(res, (double)sum / k);
        }
        return res;
    }
};
// @lc code=end

int main() {
    Solution sol;
    vector<int> nums;
    string line;
    getline(cin, line);  // 读取一整行输入
    istringstream iss(line);
    int x;
    // 只解析本行内所有整数
    while (iss >> x)
        nums.push_back(x);
    int k;
    cin >> k;
    double res = sol.findMaxAverage(nums, k);
    cout << res;
    return 0;
}