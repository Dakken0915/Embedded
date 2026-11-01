/*
 * @lc app=leetcode.cn id=236 lang=cpp
 *
 * [236] 二叉树的最近公共祖先
 */

// @lc code=start
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
private:
    TreeNode* recur(TreeNode* root, TreeNode* p, TreeNode* q)
    {
        if(!root)
            return nullptr;
        if(root == p || root == q)
            return root;
        
        TreeNode* a = recur(root->left, p, q);
        TreeNode* b = recur(root->right, p, q);

        // 关键判断
        if(a && b) return root;
        else if(a || b) return a ? a : b;
        return nullptr; 
    }

public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
       return recur(root, p, q);
    }
};
// @lc code=end

