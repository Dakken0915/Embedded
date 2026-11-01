/*
 * @lc app=leetcode.cn id=687 lang=cpp
 *
 * [687] 最长同值路径
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
        if (!root) return 0;

        int left = dfs(root->left, maxLen);
        int right = dfs(root->right, maxLen);

        int l = 0, r = 0;
        // 左孩子值相同，链可以延续
        if (root->left && root->left->val == root->val) {
            l = left + 1;
        }
        // 右孩子值相同
        if (root->right && root->right->val == root->val) {
            r = right + 1;
        }
        // 当前节点作为拐点，左右拼接，更新全局最大值
        maxLen = max(maxLen, l + r);

        // 返回单链给父节点，只能选一边
        return max(l, r);
    }

    int longestUnivaluePath(TreeNode* root) {
        recur(root);
        return maxLen;
    }
};
// @lc code=end

