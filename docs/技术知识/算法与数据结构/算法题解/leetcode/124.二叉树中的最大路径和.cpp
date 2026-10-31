/*
 * @lc app=leetcode.cn id=124 lang=cpp
 *
 * [124] 二叉树中的最大路径和
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
    int maxVal = INT_MIN;

    int recur(TreeNode* root, int preSum) {
        if(!root) return 0;

        int a = recur(root->left, preSum + root->val);
        int b = recur(root->right, preSum + root->val);

        maxVal = max(a + b + root->val, maxVal);
        // 传递的是 >0 的单根枝叶的和或0
        return max(max(a, b) + root->val, 0);
    }
public:
    int maxPathSum(TreeNode* root) {
        maxVal = root->val;
        recur(root, 0);
        return maxVal;
    }
};
// @lc code=end

