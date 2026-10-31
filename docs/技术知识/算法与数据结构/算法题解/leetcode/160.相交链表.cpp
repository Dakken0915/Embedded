/*
 * @lc app=leetcode.cn id=160 lang=cpp
 *
 * [160] 相交链表
 */

// @lc code=start
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode *A = headA;

        ListNode *B = headB;

        while(A || B) {
            if(A == nullptr) A = headB;
            if(B == nullptr) B = headA;
            if(A == B) return A;
            A = A->next;
            B = B->next;
        }
        return nullptr;
    }
};
// @lc code=end

class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        int i = 0;
        ListNode *A = headA;
        while(A) {
            i++;
            A = A->next;
        }
        int j = 0;
        ListNode *B = headB;
        while(B) {
            j++;
            B = B->next;
        }

        A = headA;
        B = headB;
        if(j - i >= 0)
            for(int k = 0; k < j - i; k++)
                B = B->next;
        else
            for(int k = 0; k < i - j; k++)
                A = A->next;
        while(A && B) {
            if(A == B) return A;
            A = A->next;
            B = B->next;
        }

        return nullptr;
    }
};