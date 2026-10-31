    /*
    * @lc app=leetcode.cn id=1449 lang=cpp
    *
    * [1449] 数位成本和为目标值的最大数字
    */

    // @lc code=start
    class Solution {
    public:
        // 返回 true 表示 str2 较大
        bool stringCompare(string str1, string str2) {
            if(str1.size() != str2.size())
                return str1.size() < str2.size();
            // 字符串长度相等
            int index = 0;
            while(index < str1.size()) {
                if(str1[index] - '0' != str2[index] - '0')
                    return str1[index] - '0' < str2[index] - '0';
                index++;  // 字符相等，对比下一个
            }
            return true;  //相等，返回任意一个皆可
        }

        string largestNumber(vector<int>& cost, int target) {
            // 注意只有 dp[0] 的空字符串是可取的
            vector<string> dp(target + 1);
            int n = cost.size();

            for(int i = 0; i < n; i++) {  // item
                int reduce = cost[i];
                int val = i + 1;
                for(int j = 0; j <= target; j++) {
                    // 字符串不可达，直接跳过
                    if(j - reduce < 0 || (j - reduce > 0 && dp[j - reduce].empty())) continue;
                    // dp[j] 存的值较大
                    if(dp[j].size() > dp[j - reduce].size() + 1)
                        continue;
                    else {
                        // 加在前面
                        string str = to_string(val) + dp[j - reduce];
                        if(stringCompare(dp[j], str))
                            dp[j] = str;
                        else
                            continue;
                    }
                }
            }
            if(dp[target].empty()) return "0";
            return dp[target];
        }
    };
    // @lc code=end

