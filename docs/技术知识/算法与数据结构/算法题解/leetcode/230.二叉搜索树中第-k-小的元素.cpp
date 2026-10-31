/*
 * @lc app=leetcode.cn id=230 lang=cpp
 *
 * [230] 二叉搜索树中第 K 小的元素
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
private:
    int cnt = 0;
    int res = 0;
    void recur(TreeNode* root, int k) {
        if(!root) return;
        recur(root->left, k);
        cnt++;
        if(cnt == k)
            res = root->val;
        recur(root->right, k);
    }

public:
    int kthSmallest(TreeNode* root, int k) {
        if(!root) return 0;
        recur(root, k);
        return res;
    }
};
// @lc code=end

