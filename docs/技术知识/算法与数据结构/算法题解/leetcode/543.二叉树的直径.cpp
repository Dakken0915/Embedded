/*
 * @lc app=leetcode.cn id=543 lang=cpp
 *
 * [543] 二叉树的直径
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
public:
    int maxLen = 0;
    int recur(TreeNode* root) {
        if(!root) return 0;
        int maxLeft = recur(root->left);
        int maxRight = recur(root->right);
        // 在递归过程中更新全局变量
        maxLen = max(maxLen, maxLeft + maxRight);
        return 1 + max(maxLeft, maxRight);
    }

    int diameterOfBinaryTree(TreeNode* root) {
        recur(root);
        return maxLen;
    }
};
// @lc code=end

