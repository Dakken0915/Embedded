#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

/*
 * @lc app=leetcode.cn id=725 lang=cpp
 *
 * [725] 分隔链表
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
    vector<ListNode*> splitListToParts(ListNode* head, int k) {
        vector<ListNode*> res(k, nullptr);
        vector<ListNode*> tail(k, nullptr); // 存每个分段的尾节点
        ListNode* cur = head;
        int n = 0; // 存储链表长度
        while (cur) {
            cur = cur->next;
            n++;
        }
        int a = n / k;
        int b = n % k;
        for (int i = 0; i < b; i++) { // 前 b 组
            for (int j = 0; j < a + 1; j++) {
                if (res[i] == nullptr) {
                    res[i] = head;
                    tail[i] = head;
                }
                else {
                    tail[i]->next = head;
                    tail[i] = tail[i]->next;
                }
                head = head->next;
            }
        }
        for (int i = b; i < k; i++) { // 后 k - b 组
            for (int j = 0; j < a; j++) {
                if (res[i] == nullptr) {
                    res[i] = head;
                    tail[i] = head;
                }
                else {
                    tail[i]->next = head;
                    tail[i] = tail[i]->next;
                }
                head = head->next;
            }
        }
        for(int i = 0; i < k; i++) {  // !!! 断开链表尾部
            if(tail[i])
                tail[i]->next = nullptr;
        }
        return res;
    }
};
// @lc code=end

int main() {
    Solution sol;
    string s;
    getline(cin, s);
    istringstream iss(s);
    int x;

    ListNode* dummy = new ListNode(); // 虚拟节点
    ListNode* tail = dummy; // 尾指针，持续往后追加节点
    while(iss >> x) {
        tail->next = new ListNode(x);
        tail = tail->next;
    }
    // 真正链表头，跳过虚拟节点
    ListNode* head = dummy->next;
    delete dummy; // 释放没用的虚拟节点

    int k;
    cin >> k;

    vector<ListNode*> res = sol.splitListToParts(head, k);
    for (int i = 0; i < res.size(); i++) {
        ListNode* cur = res[i];
        while (cur) {
            cout << cur->val;
            cur = cur->next;
        }
    }
    return 0;
}