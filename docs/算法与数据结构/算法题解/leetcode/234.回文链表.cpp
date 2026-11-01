/*
 * @lc app=leetcode.cn id=234 lang=cpp
 *
 * [234] 回文链表
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
    bool isPalindrome(ListNode* head) {
        stack<int> st;
        ListNode* slow = head;
        ListNode* fast = head;
        while(fast->next && fast->next->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        if(fast->next) {
            slow = slow->next;
        }

        // 反转链表
        ListNode* pre = nullptr;
        while(slow) {
            ListNode* temp = slow->next;  // 保存下一个节点
            slow->next = pre;
            // 两个指针同时移动
            pre = slow;
            slow = temp;
        }
        slow = pre;

        // 从两个头同时开始遍历
        while(slow && head) {
            if(slow->val != head->val)
                return false;
            slow = slow->next;
            head = head->next;
        }
        
        return true;
    }
};
// @lc code=end

// 采用栈：空间复杂度为 O(n)
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        stack<int> st;
        ListNode* temp = head;
        while(temp) {
            st.push(temp->val);
            temp = temp->next;
        }
        while(head) {
            if(head->val != st.top()) return false;
            st.pop();
            head = head->next;
        }
        return true;
    }
};