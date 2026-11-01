/*
 * @lc app=leetcode.cn id=1019 lang=cpp
 *
 * [1019] 链表中的下一个更大节点
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
    vector<int> nextLargerNodes(ListNode* head) {
        stack<pair<int, int>> st;  // 单调栈

        int n = 0;
        ListNode* head1 = head;
        while(head1) {
            n++;
            head1 = head1->next;
        }

        vector<int> res(n, 0);
        int index = 0;
        while(head) {
            while(!st.empty() && head->val > st.top().second) {
                res[st.top().first] = head->val;
                st.pop();
            }

            st.push({index, head->val});
            // 遍历下一项
            index++;
            head = head->next;
        }
        return res;
    }
};
// @lc code=end

