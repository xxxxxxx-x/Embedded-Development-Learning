/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0806/demo1/main.c
 * @brief        : 实现二叉树的前序、中序、后序遍历（递归方式）
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-06 13:23:04
 * @version      : 1.0
 * @note         :
 *                本程序演示了如何使用递归方式对二叉树进行前序、中序和后序遍历。
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Rights Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>

// 定义二叉树节点结构体
typedef struct TreeNode {
  int value;              // 节点的值
  struct TreeNode *left;  // 指向左子节点的指针
  struct TreeNode *right; // 指向右子节点的指针
} TreeNode;

// 创建新节点的函数
TreeNode *createNode(int value) {
  TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
  if (newNode == NULL) {
    printf("内存分配失败\n");
    exit(1);
  }
  newNode->value = value;
  newNode->left = NULL;
  newNode->right = NULL;
  return newNode;
}

// 前序遍历：根 -> 左 -> 右
void preOrderTraversal(TreeNode *root) {
  if (root == NULL) {
    return;
  }
  printf("%d ", root->value);     // 访问根节点
  preOrderTraversal(root->left);  // 遍历左子树
  preOrderTraversal(root->right); // 遍历右子树
}

// 中序遍历：左 -> 根 -> 右
void inOrderTraversal(TreeNode *root) {
  if (root == NULL) {
    return;
  }
  inOrderTraversal(root->left);  // 遍历左子树
  printf("%d ", root->value);    // 访问根节点
  inOrderTraversal(root->right); // 遍历右子树
}

// 后序遍历：左 -> 右 -> 根
void postOrderTraversal(TreeNode *root) {
  if (root == NULL) {
    return;
  }
  postOrderTraversal(root->left);  // 遍历左子树
  postOrderTraversal(root->right); // 遍历右子树
  printf("%d ", root->value);      // 访问根节点
}

int main() {
  // 创建一个简单的二叉树
  TreeNode *root = createNode(1);
  root->left = createNode(2);
  root->right = createNode(3);
  root->left->left = createNode(4);
  root->left->right = createNode(5);

  printf("前序遍历: ");
  preOrderTraversal(root);
  printf("\n");

  printf("中序遍历: ");
  inOrderTraversal(root);
  printf("\n");

  printf("后序遍历: ");
  postOrderTraversal(root);
  printf("\n");

  return 0;
}
