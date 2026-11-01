/*
 * @lc app=leetcode.cn id=437 lang=cpp
 *
 * [437] 路径总和 III
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
    // preSum 用 int 可能会超出数值范围，这里使用 long long
    int dfs(TreeNode* root, int targetSum, long long preSum) {
        if(!root) return 0;
        int count = 0;
        if(preSum + root->val == targetSum) count++;
        count += dfs(root->left, targetSum, preSum + root->val);
        count += dfs(root->right, targetSum, preSum + root->val);
        return count;
    }

    int recur(TreeNode* root, int targetSum) {
        if(!root) return 0;
        int res = 0;

        res += dfs(root, targetSum, 0);
        res += recur(root->left, targetSum);
        res += recur(root->right, targetSum);
        return res;
    }

public:
    int pathSum(TreeNode* root, int targetSum) {
        return recur(root, targetSum);
    }
};
// @lc code=end

// 问题：数组拷贝超出内存
class Solution {
private:
    int recur(TreeNode* root, vector<int> arr, int targetSum) {
        if(!root) return 0;
        int res = 0;
        for(int i = 0; i < arr.size(); i++) {
            arr[i] += root->val;
            if(arr[i] == targetSum)
                res++;
        }
        if(root->val == targetSum) res++;
        arr.push_back(root->val);
        
        res += recur(root->left, arr, targetSum);
        res += recur(root->right, arr, targetSum);
        return res;
    }
public:
    int pathSum(TreeNode* root, int targetSum) {
        vector<int> arr;
        return recur(root, arr, targetSum);
    }
};