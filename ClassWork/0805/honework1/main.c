/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0805/honework1/main.c
 * @brief        : 实现一个简单的平衡二叉树，并绘制树结构
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-05 18:19:50
 * @version      : 1.2
 * @note         : 使用递归插入节点和调整树的平衡
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Rights Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

// 树节点结构体定义
typedef struct TreeNode {
  int value;              // 节点值
  struct TreeNode *left;  // 左子树
  struct TreeNode *right; // 右子树
  int height;             // 节点高度
} TreeNode;

// 获取节点高度
int getHeight(TreeNode *node) { return node ? node->height : 0; }

// 更新节点高度
void updateHeight(TreeNode *node) {
  if (node) {
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
  }
}

// 右旋
TreeNode *rotateRight(TreeNode *y) {
  TreeNode *x = y->left;
  TreeNode *T2 = x->right;
  x->right = y;
  y->left = T2;
  updateHeight(y);
  updateHeight(x);
  return x;
}

// 左旋
TreeNode *rotateLeft(TreeNode *x) {
  TreeNode *y = x->right;
  TreeNode *T2 = y->left;
  y->left = x;
  x->right = T2;
  updateHeight(x);
  updateHeight(y);
  return y;
}

// 获取平衡因子
int getBalance(TreeNode *node) {
  return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// 插入节点到树中并保持平衡
TreeNode *insertNode(TreeNode *node, int value) {
  // 1. 插入节点
  if (node == NULL) {
    node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node == NULL) {
      perror("内存分配失败");
      exit(EXIT_FAILURE);
    }
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; // 新节点的高度初始化为1
    return node;
  }

  if (value < node->value) {
    node->left = insertNode(node->left, value);
  } else if (value > node->value) {
    node->right = insertNode(node->right, value);
  } else {
    return node; // 不允许插入重复值
  }

  // 2. 更新节点高度
  updateHeight(node);

  // 3. 计算平衡因子并调整树的平衡
  int balance = getBalance(node);

  // 左左情况
  if (balance > 1 && value < node->left->value) {
    return rotateRight(node);
  }

  // 右右情况
  if (balance < -1 && value > node->right->value) {
    return rotateLeft(node);
  }

  // 左右情况
  if (balance > 1 && value > node->left->value) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
  }

  // 右左情况
  if (balance < -1 && value < node->right->value) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
  }

  return node;
}

// 中序遍历树并显示节点
void displayTree(TreeNode *root) {
  if (root == NULL) {
    return;
  }
  if (root->left) {
    displayTree(root->left);
  }

  printf("%d\n", root->value);

  if (root->right) {
    displayTree(root->right);
  }
}

int main(int argc, const char *argv[]) {
  int values[] = {10, 20, 30, 40, 50, 60, 70};
  TreeNode *root = NULL;

  // 插入数组中的所有元素到树中
  for (int i = 0; i < sizeof(values) / sizeof(int); i++) {
    root = insertNode(root, values[i]);
  }

  // 显示树节点
  printf("树的节点为:\n");
  displayTree(root);

  return 0;
}
