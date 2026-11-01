// @before-stub-for-debug-begin
#include <vector>
#include <string>
#include "commoncppproblem199.h"

using namespace std;
// @before-stub-for-debug-end

/*
 * @lc app=leetcode.cn id=199 lang=cpp
 *
 * [199] 二叉树的右视图
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
    queue<pair<int, int>> que;
    void recur(TreeNode* root, int level) {
        if(!root) return;
        que.push({root->val, level});
        recur(root->right, level + 1);
        recur(root->left, level + 1);
    }

public:
    vector<int> rightSideView(TreeNode* root) {
        recur(root, 0);
        vector<int> res;
        int index = 0;
        while(!que.empty()) {
            if(que.front().second >= index) {
                res.push_back(que.front().first);
                index++;
            }
            que.pop();
        }
        return res;
    }
};
// @lc code=end

