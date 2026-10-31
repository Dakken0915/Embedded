/*
 * @lc app=leetcode.cn id=292 lang=cpp
 *
 * [292] Nim 游戏
 */

// @lc code=start
class Solution {
public:
    bool canWinNim(int n) {
        return n % 4 != 0;
    }
};
// @lc code=end

// 问题：时间或内存超限
class Solution {
public:
    bool canWinNim(int n) {
        if(n < 4)
            return true;
        
        int base = 1;  // 先手决定拿掉的石头
        while(base <= n && base <= 3) {
            // 后手输就是先手赢
            if(canWinNim(n - base) == false)
                return true;
            
            if(base == n) break;  // 防溢出判断
            base++;
        }
        // 所有拿法后手都能赢，当前先手必败
        return false;
    }
};