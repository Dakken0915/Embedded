/*
 * @lc app=leetcode.cn id=911 lang=cpp
 *
 * [911] 在线选举
 */

// @lc code=start
class TopVotedCandidate {
public:
    vector<int> maxByTime;
    vector<int> times;

    TopVotedCandidate(vector<int>& persons, vector<int>& times) {
        unordered_map<int, int> map;
        int p = -1;
        map[p] = -1;
        for(int x : persons) {
            map[x]++;
            if(map[x] >= map[p])
                p = x;
            // 最后将每个时刻的最大值存入
            maxByTime.push_back(p);
        }
        this->times = times;
    }
    
    int q(int t) {
        auto it = upper_bound(times.begin(), times.end(), t) - times.begin() - 1;
        return maxByTime[it];
    }
};

/**
 * Your TopVotedCandidate object will be instantiated and called as such:
 * TopVotedCandidate* obj = new TopVotedCandidate(persons, times);
 * int param_1 = obj->q(t);
 */
// @lc code=end

