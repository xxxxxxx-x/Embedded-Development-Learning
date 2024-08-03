/****************************************************************************************************************************************************************
 * @file         : /Tools/home/user/Code/ClassWork/0724/demo2/main.c
 * @brief        : 小练习：定义狗的结构体，用三种不同的赋值方法生成三条狗
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-24 11:24:52
 * @version      : 1.0
 * @note         : 展示了如何用不同的方法初始化结构体
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>  // 包含输入输出头文件
#include <string.h> // 包含字符串操作头文件

// 定义描述狗的结构体
struct dog {
  char name[100];      // 狗的名字
  int age;             // 狗的年龄
  char sex;            // 狗的性别，'M' 表示雄性，'F' 表示雌性
  void (*hobby)(void); // 狗的爱好，用函数指针表示
};

// 示例的爱好函数
void bark() { printf("Woof! Woof!\n"); }

int main(int argc, const char *argv[]) {
  // 方法一：初始化时直接赋值
  struct dog dahuang = {"小黄", 3, 'M', bark};

  // 方法二：声明后逐个赋值
  struct dog xiaohei;
  strncpy(xiaohei.name, "小黑", sizeof(xiaohei.name)); // 使用 strncpy 避免溢出
  xiaohei.age = 1;
  xiaohei.sex = 'M';
  xiaohei.hobby = bark;

  // 方法三：使用指针赋值
  struct dog ahuang;
  struct dog *pAhuang = &ahuang;
  strncpy(pAhuang->name, "小白",
          sizeof(pAhuang->name)); // 使用 strncpy 避免溢出
  pAhuang->age = 2;
  pAhuang->sex = 'F';
  pAhuang->hobby = bark;

  // 打印狗的信息并调用其爱好函数
  printf("狗的名字: %s\n", dahuang.name);
  printf("狗的年龄: %d\n", dahuang.age);
  printf("狗的性别: %c\n", dahuang.sex);
  dahuang.hobby();

  printf("狗的名字: %s\n", xiaohei.name);
  printf("狗的年龄: %d\n", xiaohei.age);
  printf("狗的性别: %c\n", xiaohei.sex);
  xiaohei.hobby();

  printf("狗的名字: %s\n", ahuang.name);
  printf("狗的年龄: %d\n", ahuang.age);
  printf("狗的性别: %c\n", ahuang.sex);
  ahuang.hobby();

  return 0; // 返回 0 表示程序正常结束
}
