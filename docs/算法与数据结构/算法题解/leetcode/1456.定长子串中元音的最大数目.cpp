#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxVowels(string s, int k) {
        if(s.empty()) return 0;
        int n = s.size();
        int res = 0;
        int r = 0;   // 临时统计
        for(int i = 0; i < n; i++){
            if(s[i] == 'a' || s[i] == 'e' || s[i] == 'i' 
                    || s[i] == 'o' || s[i] == 'u')
                r++;
            if(i >= k)
                if(s[i - k] == 'a' || s[i - k] == 'e' || s[i - k] == 'i' 
                    || s[i - k] == 'o' || s[i - k] == 'u')
                    r--;

            res = max(res, r);
        }
        return res;
    }
};

int main() {
    Solution sol;
    string s;
    int k;
    cin >> s >> k;
    int res = sol.maxVowels(s, k);
    cout << res << endl;
}