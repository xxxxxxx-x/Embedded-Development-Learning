/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0809/homework3/dclist.c
 * @brief        : 示例代码展示了如何使用内核链表进行基本的链表操作，节点数据为文件路径
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-05 10:14:07
 * @version      : 1.0
 * @note         : 处理节点数据为文件路径，提供链表操作功能
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Rights Reserved
 *****************************************************************************************************************************************************************/

#include "dclist.h"

/****************************************************************************************************************************************************************
 * @name:        create_node
 * @brief:       创建并初始化一个新的节点
 * @param {char*} path: 节点要存储的文件路径
 * @return:      新创建的节点指针，如果失败则返回 NULL
 * @date:        2024-08-05 14:30:49
 * @version:     1.0
 * @note:        分配内存并初始化链表节点
 *****************************************************************************************************************************************************************/
ImageList *create_node(const char *path) {
  // 分配内存，创建新的节点
  ImageList *new_node = malloc(sizeof(ImageList));
  if (!new_node) {
    // 如果内存分配失败，打印错误信息并返回 NULL
    printf("内存分配失败\n");
    return NULL;
  }

  // 复制文件路径到节点
  strncpy(new_node->filepath, path, sizeof(new_node->filepath) - 1);
  new_node->filepath[sizeof(new_node->filepath) - 1] = '\0'; // 确保字符串以null结尾

  // 初始化链表节点
  INIT_LIST_HEAD(&new_node->list);
  return new_node;
}

/****************************************************************************************************************************************************************
 * @name:        insert_node
 * @brief:       将新节点插入到链表的尾部
 * @param {list_head*} head: 链表头指针
 * @param {char*} path: 节点要存储的文件路径
 * @return:      无
 * @date:        2024-08-05 14:30:55
 * @version:     1.0
 * @note:        创建节点并将其添加到链表尾部
 *****************************************************************************************************************************************************************/
void insert_node(struct list_head *head, const char *path) {
  // 创建新节点
  ImageList *new_node = create_node(path);
  if (!new_node)
    return; // 如果节点创建失败，退出函数

  // 将新节点添加到链表的尾部
  list_add_tail(&new_node->list, head);
}

/****************************************************************************************************************************************************************
 * @name:        show_nodes
 * @brief:       遍历并显示链表中的所有节点
 * @param {list_head*} head: 链表头指针
 * @return:      无
 * @date:        2024-08-05 14:31:12
 * @version:     1.0
 * @note:        遍历链表并打印每个节点的数据
 *****************************************************************************************************************************************************************/
void show_nodes(struct list_head *head) {
  struct list_head *pos;

  // 遍历链表
  LIST_FOR_EACH(pos, head) {
    // 根据链表节点指针获取包含数据的结构体指针
    ImageList *node = LIST_ENTRY(pos, ImageList, list);

    // 打印节点文件路径
    printf("节点文件路径: %s\n", node->filepath);
  }
}

/****************************************************************************************************************************************************************
 * @name:        delete_node
 * @brief:       从链表中删除指定文件路径的节点
 * @param {list_head*} head: 链表头指针
 * @param {char*} del_path: 要删除的节点文件路径
 * @return:      无
 * @date:        2024-08-05 14:31:21
 * @version:     1.0
 * @note:        遍历链表找到并删除指定文件路径的节点
 *****************************************************************************************************************************************************************/
void delete_node(struct list_head *head, const char *del_path) {
  struct list_head *pos, *n;

  // 遍历链表
  LIST_FOR_EACH_SAFE(pos, n, head) {
    // 根据链表节点指针获取包含数据的结构体指针
    ImageList *node = LIST_ENTRY(pos, ImageList, list);

    // 如果找到匹配的文件路径，删除该节点
    if (strcmp(node->filepath, del_path) == 0) {
      list_del(pos); // 从链表中删除节点
      free(node);    // 释放节点内存
      return;        // 删除成功后退出函数
    }
  }
}

/****************************************************************************************************************************************************************
 * @name:        find_node
 * @brief:       查找链表中包含指定文件路径的节点
 * @param {list_head*} head: 链表头指针
 * @param {char*} search_path: 要查找的文件路径
 * @return:      包含指定文件路径的节点指针，如果未找到返回 NULL
 * @date:        2024-08-05 14:31:29
 * @version:     1.0
 * @note:        遍历链表查找并返回匹配的文件路径节点
 *****************************************************************************************************************************************************************/
ImageList *find_node(struct list_head *head, const char *search_path) {
  struct list_head *pos;

  // 遍历链表
  LIST_FOR_EACH(pos, head) {
    // 根据链表节点指针获取包含数据的结构体指针
    ImageList *node = LIST_ENTRY(pos, ImageList, list);

    // 如果找到匹配的文件路径，返回节点指针
    if (strcmp(node->filepath, search_path) == 0) {
      return node;
    }
  }

  return NULL; // 如果未找到，返回 NULL
}

/****************************************************************************************************************************************************************
 * @name:        modify_node
 * @brief:       修改链表中指定文件路径节点的值
 * @param {list_head*} head: 链表头指针
 * @param {char*} old_path: 要修改的节点的旧文件路径
 * @param {char*} new_path: 新的文件路径
 * @return:      无
 * @date:        2024-08-05 14:31:36
 * @version:     1.0
 * @note:        查找节点并修改其文件路径值
 *****************************************************************************************************************************************************************/
void modify_node(struct list_head *head, const char *old_path, const char *new_path) {
  // 查找包含旧文件路径的节点
  ImageList *node = find_node(head, old_path);
  if (node == NULL) {
    printf("文件路径未找到\n"); // 如果未找到，打印错误信息
  } else {
    // 修改节点文件路径
    strncpy(node->filepath, new_path, sizeof(node->filepath) - 1);
    node->filepath[sizeof(node->filepath) - 1] = '\0'; // 确保字符串以null结尾
  }
}

/****************************************************************************************************************************************************************
 * @name:        list_splice_tail
 * @brief:       将源链表的节点拼接到目标链表的尾部
 * @param {list_head*} source: 源链表
 * @param {list_head*} target: 目标链表
 * @return:      无
 * @date:        2024-08-05 14:31:43
 * @version:     1.0
 * @note:        将源链表中的所有节点移动到目标链表的尾部
 *****************************************************************************************************************************************************************/
void list_splice_tail(struct list_head *source, struct list_head *target) {
  if (list_empty(source)) // 如果源链表为空，直接返回
    return;

  struct list_head *first = source->next; // 源链表第一个节点
  struct list_head *last = source->prev;  // 源链表最后一个节点

  // 更新目标链表的链接指针，将源链表拼接到目标链表
  target->prev->next = first;
  first->prev = target->prev;
  last->next = target;
  target->prev = last;

  // 初始化源链表为空链表
  INIT_LIST_HEAD(source);
}

// 示例主函数（已注释，可以根据需要解开并修改）
// int main() {
//   struct list_head head;
//   INIT_LIST_HEAD(&head);

//   // 示例文件路径
//   const char *path1 = "/path/to/image1.jpg";
//   const char *path2 = "/path/to/image2.jpg";

//   insert_node(&head, path1);
//   insert_node(&head, path2);

//   show_nodes(&head);

//   // 查找、修改、删除节点的操作可以在这里添加

//   return 0;
// }
