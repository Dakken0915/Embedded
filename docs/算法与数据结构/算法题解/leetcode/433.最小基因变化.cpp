/*
 * @lc app=leetcode.cn id=433 lang=cpp
 *
 * [433] 最小基因变化
 */

// @lc code=start
class Solution {
public:
    int minMutation(string startGene, string endGene, vector<string>& bank) {
        unordered_set<string> visited;
        unordered_set<string> count;
        char keys[4] = {'A', 'C', 'G', 'T'};  
        
        // 建立哈希表，方便查找
        for(string& gene : bank)
            count.insert(gene);

        if(startGene == endGene) return 0;
        if(count.find(endGene) == count.end()) return -1;

        // BFS
        queue<string> que;
        que.push(startGene);

        int step = 0;
        while(!que.empty()) {
            int sz = que.size();
            // 处理这一层所有节点，方便计算步数
            step++;
            for (int k = 0; k < sz; k++) {
                // BFS模板操作
                string x = que.front();
                visited.insert(x);  // 标记已访问
                que.pop();

                // 逐个字母变化尝试
                for(int i = 0; i < 8; i++) {
                    for(int j = 0; j < 4; j++) {
                        if(x[i] != keys[j]) {
                            // 拷贝副本，判断是否可以入队
                            string copy = x;
                            copy[i] = keys[j];

                            // 找到目标
                            if(copy == endGene) return step;
                            
                            // 能找到基因序列且未访问过
                            if(count.find(copy) != count.end() 
                            && visited.find(copy) == visited.end()) {
                                que.push(copy);
                            }
                        }
                    }
                }
            }
        }
        return -1;
    }
};
// @lc code=end

