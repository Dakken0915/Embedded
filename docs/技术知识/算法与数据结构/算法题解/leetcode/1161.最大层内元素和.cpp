/*
 * @lc app=leetcode.cn id=1161 lang=cpp
 *
 * [1161] 最大层内元素和
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

// BFS解法
class Solution {
public:
    int maxLevelSum(TreeNode* root) {
        int res = 1;
        int maxSum = INT_MIN;
        queue<TreeNode*> que;
        que.push(root);
        for(int level = 1; !que.empty(); level++) {
            int size = que.size();
            int temp = 0;
            // 开始处理这一层
            for(int i = 0; i < size; i++) {
                TreeNode* cur = que.front();
                temp += cur->val;
                if(cur->left)
                    que.push(cur->left);
                if(cur->right)
                    que.push(cur->right);
                que.pop();
            }
            // 当前层结束，更新答案
            if(temp > maxSum) {
                res = level;
                maxSum = temp;
            }
        }
        return res;
    }
};
// @lc code=end

class Solution {
public:
    int maxLevelSum(TreeNode* root) {
        queue<pair<TreeNode*, int>> que;
        que.push({root, 0});
        int level = 0;
        int temp = 0;
        vector<int> levelsum;
        while(!que.empty()) {
            TreeNode* curNode = que.front().first;
            int curLevel = que.front().second;
            if(curLevel == level)
                temp += curNode->val;
            else {
                // 将每层的总和记录下来
                levelsum.push_back(temp);
                temp = curNode->val;
                level = curLevel;  // 更新层数
            }
            que.pop();
            if(curNode->left)
                que.push({curNode->left, curLevel + 1});
            if(curNode->right)
                que.push({curNode->right, curLevel + 1});
        }
        levelsum.push_back(temp); // 记录最后一层

        int maxSum = INT_MIN;
        int res = 1;
        for(int i = 0; i < levelsum.size(); i++) {
            if(levelsum[i] > maxSum) {
                res = i + 1;
                maxSum = levelsum[i];
            }
        }
        return res;
    }
};
