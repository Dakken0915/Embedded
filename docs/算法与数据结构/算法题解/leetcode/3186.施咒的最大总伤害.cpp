/*
 * @lc app=leetcode.cn id=3186 lang=cpp
 *
 * [3186] 施咒的最大总伤害
 */

// @lc code=start
class Solution {
public:
    long long maximumTotalDamage(vector<int>& power) {
        if (power.empty()) return 0;

        // 统计每种法术值的总伤害
        unordered_map<int, long long> freq;
        for (int x : power)
            freq[x] += x;  // 直接累加伤害值（伤害 = 法术值 * 出现次数）


        // 提取所有出现的法术值并排序
        vector<int> keys;
        for (auto& p : freq)
            keys.push_back(p.first);

        sort(keys.begin(), keys.end());

        int n = keys.size();
        vector<long long> dp(n, 0);

        // 初始化前三个状态（逻辑与你原来一致）
        dp[0] = freq[keys[0]];
        if (n == 1) return dp[0];

        // 处理第二个：如果相邻（差值为1或2），选最大值；否则可以都取
        if (keys[1] - keys[0] <= 2)
            dp[1] = max(dp[0], freq[keys[1]]);
        else
            dp[1] = dp[0] + freq[keys[1]];

        if (n == 2) return dp[1];

        // 处理第三个
        dp[2] = freq[keys[2]];
        if (keys[2] - keys[1] > 2)
            dp[2] += dp[1];
        else if (keys[2] - keys[0] > 2)
            dp[2] += dp[0];

        dp[2] = max(dp[2], dp[1]);  // 保证 dp[i] 是到 i 为止的最大值
        if (n == 3) return dp[2];

        // 处理后续：类似打家劫舍，但差值为 1 或 2 的都视为"相邻"
        for (int i = 3; i < n; i++) {
            // 如果不选当前法术，继承前一个最优
            dp[i] = dp[i - 1];
            
            // 如果选当前法术，找到第一个不相邻的位置
            int j = i - 1;
            while (j >= 0 && keys[i] - keys[j] <= 2)
                j--;
            
            long long take = freq[keys[i]];
            if (j >= 0) take += dp[j];
            dp[i] = max(dp[i], take);
        }
        return dp[n - 1];
    }
};
// @lc code=end

// 出现内存超出限制问题, 原因：maxPower过大
class Solution {
public:
    long long maximumTotalDamage(vector<int>& power) {
        if(!power.size()) return 0;
        // 转换为打家劫舍问题
        unordered_map<int, int> mp;
        int maxPower = INT_MIN;
        for(int x : power) {
            mp[x] += 1;
            maxPower = max(maxPower, x);
        }
        vector<long long> damage(maxPower, 0);
        for(int i = 0; i < maxPower; i++)
            damage[i] = mp[i + 1];

        vector<long long> dp(maxPower, 0);
        // 初始化
        // 注意这里判断条件不要写成 power.size()
        if(maxPower == 1) return damage[0];
        if(maxPower == 2) return max(damage[0], 2 * damage[1]);
        if(maxPower == 3) return max(damage[0], max(2 * damage[1], 3 * damage[2]));

        // 注意是前一项的 dp 用来比较，不是 damge
        dp[0] = damage[0];
        dp[1] = max(dp[0], 2 * damage[1]);
        dp[2] = max(dp[1], 3 * damage[2]);
        for(int i = 3; i < maxPower; i++)
            dp[i] = max(damage[i] + dp[i - 3], dp[i - 1]);

        return dp[maxPower - 1];
    }
};