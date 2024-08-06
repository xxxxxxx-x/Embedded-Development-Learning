/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0806/demo2/main.c
 * @brief        : 实现二叉树的元素插入（递归方式）
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-06 13:37:32
 * @version      : 1.0
 * @note         : 本程序演示了如何递归地向二叉树中插入新元素。
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

// 插入节点的递归函数
TreeNode *insertNode(TreeNode *root, int value) {
  // 如果当前节点为空，插入新节点
  if (root == NULL) {
    return createNode(value);
  }

  // 如果值小于当前节点的值，插入到左子树
  if (value < root->value) {
    root->left = insertNode(root->left, value);
  }
  // 如果值大于等于当前节点的值，插入到右子树
  else {
    root->right = insertNode(root->right, value);
  }

  return root; // 返回当前节点
}

// 中序遍历：左 -> 根 -> 右（用于测试插入的顺序）
void inOrderTraversal(TreeNode *root) {
  if (root == NULL) {
    return;
  }
  inOrderTraversal(root->left);  // 遍历左子树
  printf("%d ", root->value);    // 访问根节点
  inOrderTraversal(root->right); // 遍历右子树
}

int main() {
  // 创建二叉树的根节点
  TreeNode *root = NULL;

  // 向树中插入节点
  root = insertNode(root, 50);
  root = insertNode(root, 30);
  root = insertNode(root, 70);
  root = insertNode(root, 20);
  root = insertNode(root, 40);
  root = insertNode(root, 60);
  root = insertNode(root, 80);

  // 打印树的中序遍历结果
  printf("二叉树的中序遍历: ");
  inOrderTraversal(root);
  printf("\n");

  return 0;
}
