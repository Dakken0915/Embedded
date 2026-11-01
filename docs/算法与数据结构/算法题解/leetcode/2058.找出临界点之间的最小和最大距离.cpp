/*
 * @lc app=leetcode.cn id=2058 lang=cpp
 *
 * [2058] 找出临界点之间的最小和最大距离
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    vector<int> nodesBetweenCriticalPoints(ListNode* head) {
        vector<int> ans(2, -1);
        // 边界：节点少于3个不可能有临界点
        if (!head || !head->next || !head->next->next)
            return ans;

        int minDis = INT_MAX;
        int firstIdx = -1;   // 第一个临界点下标
        int lastPreIdx = -1; // 上一个临界点下标
        int curIdx = 1;       // 当前节点下标（从第二个节点开始，i=1）

        ListNode* pre = head;
        ListNode* cur = head->next;
        ListNode* nxt = cur->next;

        while (nxt) {
            // 判断是否是临界点：极大/极小值
            bool isCritical = (cur->val < pre->val && cur->val < nxt->val)
                            || (cur->val > pre->val && cur->val > nxt->val);
            if (isCritical) {
                if (firstIdx == -1) {
                    // 记录第一个临界点
                    firstIdx = curIdx;
                } else {
                    // 计算和上一个临界点的距离，更新最小值
                    minDis = min(minDis, curIdx - lastPreIdx);
                }
                lastPreIdx = curIdx;
            }
            // 三指针同步后移
            pre = cur;
            cur = nxt;
            nxt = nxt->next;
            curIdx++;
        }

        // 临界点数量不足2个，直接返回[-1,-1]
        if (firstIdx == lastPreIdx)
            return ans;

        ans[0] = minDis;
        ans[1] = lastPreIdx - firstIdx;
        return ans;
    }
};
// @lc code=end

vector<int> nodesBetweenCriticalPoints(ListNode* head) {
        vector<int> index;  // 记录临界点的索引
        vector<int> val;    // 记录所有链表的值
        vector<int> res(2, -1);
        int minDistance = INT_MAX; // #include <climits>
        int maxDistance = 0;
        while(head) {
            val.push_back(head->val);
            head = head->next;
        }
        for(int i = 1; i < val.size() - 1; i++) {
            if((val[i] < val[i - 1] && val[i] < val[i + 1]) 
                || (val[i] > val[i - 1] && val[i] > val[i + 1])) {
                index.push_back(i);
            }
        }
        if(index.size() < 2) return res;
        for(int i = 1; i < index.size(); i++) {
            minDistance = min(minDistance, index[i] - index[i - 1]);
        }
        maxDistance = index[index.size() - 1] - index[0];
        res[0] = minDistance;
        res[1] = maxDistance;
        return res;
    }