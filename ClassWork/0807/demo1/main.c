/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0807/demo1/main.c
 * @brief        : 小练习：实现哈希表存储和查找姓名
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-07 14:45:50
 * @version      : 1.1
 * @note         :
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Right Reserved
 *****************************************************************************************************************************************************************/

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义节点结构体，用于存储每个名字
struct Node {
  char name[128];    // 存储名字
  struct Node *next; // 指向下一个节点的指针
};

// 定义哈希表的大小
#define HASH_SIZE 26

// 初始化哈希表头指针数组
void init_hash(struct Node *hash_table[HASH_SIZE]) {
  for (int i = 0; i < HASH_SIZE; i++) {
    hash_table[i] = NULL; // 将每个哈希槽初始化为空
  }
}

// 获取中文姓氏的拼音首字母，映射到哈希表的索引
char get_hash(const char *name) {
  if (name == NULL || name[0] == '\0') {
    return '?'; // 对于空名字返回 '?'
  }

  setlocale(LC_CTYPE, ""); // 设置本地环境以支持宽字符
  wchar_t wide_name[2];
  mbstowcs(wide_name, name, 2); // 仅转换前两个字节

  // 通过拼音首字母映射到哈希表的索引
  switch (wide_name[0]) {
  case L'曹':
  case L'陈':
    return 'C';
  case L'曾':
  case L'郑':
    return 'Z';
  case L'刘':
  case L'李':
  case L'黎':
  case L'梁':
  case L'罗':
    return 'L';
  case L'涂':
  case L'唐':
  case L'田':
    return 'T';
  case L'何':
  case L'黄':
  case L'胡':
    return 'H';
  case L'马':
  case L'莫':
    return 'M';
  case L'苏':
    return 'S';
  case L'熊':
    return 'X';
  case L'王':
  case L'吴':
    return 'W';
  case L'江':
    return 'J';
  case L'严':
  case L'尹':
    return 'Y';
  case L'钟':
    return 'Z';
  case L'郭':
    return 'G';
  case L'潘':
    return 'P';
  default:
    return '?'; // 对于不在字典中的姓氏返回 '?'
  }
}

// 插入名字到哈希表中
void insert_value(struct Node *hash_table[HASH_SIZE], const char name[128]) {
  struct Node *new_node = malloc(sizeof(struct Node));
  if (new_node == NULL) {
    fprintf(stderr, "内存分配失败\n");
    exit(EXIT_FAILURE);
  }
  strcpy(new_node->name, name);
  new_node->next = NULL;

  // 计算哈希值
  char hash_index = get_hash(name) - 'A';
  if (hash_index < 0 || hash_index >= HASH_SIZE) {
    free(new_node);
    return;
  }

  // 插入到哈希表中
  if (hash_table[hash_index] == NULL) {
    hash_table[hash_index] = new_node; // 如果哈希槽为空，直接插入
  } else {
    struct Node *current = hash_table[hash_index];
    while (current->next != NULL) {
      current = current->next; // 遍历到链表末尾
    }
    current->next = new_node; // 插入新节点
  }
}

// 显示哈希表中的所有名字
void show_hash(struct Node *hash_table[HASH_SIZE]) {
  for (int i = 0; i < HASH_SIZE; i++) {
    struct Node *current = hash_table[i];
    if (current != NULL) {
      printf("哈希槽 %c: ", 'A' + i);
      while (current != NULL) {
        printf("%s ", current->name);
        current = current->next;
      }
      printf("\n");
    } else {
      printf("哈希槽 %c: 为空\n", 'A' + i);
    }
  }
}

// 查找哈希表中的名字
int find_hash(struct Node *hash_table[HASH_SIZE], const char *name) {
  char hash_index = get_hash(name) - 'A';
  if (hash_index < 0 || hash_index >= HASH_SIZE) {
    printf("无法识别的姓氏\n");
    return -1;
  }

  struct Node *current = hash_table[hash_index];
  while (current != NULL) {
    if (strncmp(current->name, name, strlen(name)) == 0) {
      printf("找到名字: %s\n", current->name);
      return 1; // 找到返回1
    }
    current = current->next;
  }
  printf("没有找到名字: %s\n", name);
  return 0; // 没找到返回0
}

int main() {
  struct Node *hash_table[HASH_SIZE];

  // 初始化哈希表
  init_hash(hash_table);

  // 定义学生名单的二维数组
  char students[][50][128] = {
      {"曹文明", "曾天宇", "曾伟健", "陈钏艺", "陈建杭", "陈键杭", "陈棨良",
       "陈伟源", "陈永钊"},
      {"董靖媛"},
      {"高湧杰", "龚浩", "龚文乐", "郭飞宇"},
      {"何俊林", "何智兴", "胡世豪", "黄耿武", "黄佳和", "黄满", "黄千一",
       "黄伟涛", "黄鑫", "黄毅俊", "黄永锐"},
      {"江文卓"},
      {"康江源"},
      {"赖鹏伦", "黎粮榜", "李佳炜", "李嘉炜", "李启培", "李伟平", "李义",
       "李宇斌", "李梓艳", "梁宏伟", "梁桦澍", "梁钊杨", "廖泳鑫", "林锦淼",
       "林明杰", "林毅鸿", "林子乐", "刘浩波", "刘善鑫", "刘威",   "刘鑫",
       "刘志豪", "卢果",   "卢家吉", "罗逸富", "罗梓晴", "骆宏业"},
      {"马至权", "莫灿苗"},
      {"潘凉昌"},
      {"秦云影"},
      {"申缘", "宋维民", "苏俊博"},
      {"谭家龙", "谭燕萍", "谭正言", "唐金山", "唐梓莹", "田冠杰", "涂建泉"},
      {"王顺祖", "魏子盛", "吴晓正"},
      {"熊春节"},
      {"严锦彬", "杨路坦", "杨星", "叶智健", "尹涛", "于佳文"},
      {"张光辉", "张宇镔", "郑坚帆", "钟清", "钟少钦", "钟震伦", "周国栋",
       "周继兴", "朱锦康"}};

  // 插入学生名字到哈希表
  for (int i = 0; i < sizeof(students) / sizeof(students[0]); i++) {
    for (int j = 0; j < sizeof(students[i]) / sizeof(students[i][0]); j++) {
      if (students[i][j][0] != '\0') {            // 判断名字是否为空
        insert_value(hash_table, students[i][j]); // 插入名字到哈希表
      }
    }
  }

  // 显示哈希表
  show_hash(hash_table);

  // 查找名字 "陈钏艺"
  find_hash(hash_table, "陈钏艺");

  // 释放内存（略，实际中应该遍历释放所有链表节点）

  return 0;
}
