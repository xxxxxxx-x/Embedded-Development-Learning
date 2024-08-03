/****************************************************************************************************************************************************************
 * @file         : /Tools/home/user/Code/ClassWork/0724/demo1/main.c
 * @brief        : 小练习：定义一个描述狗的结构体
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-24 10:56:51
 * @version      : 1.0
 * @note         :
 *                  定义了一个描述狗的结构体，包括名字、年龄、性别和爱好（使用函数指针表示）
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

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

  // 定义一个狗的实例
  struct dog myDog;

  // 初始化狗的属性
  snprintf(myDog.name, sizeof(myDog.name), "Buddy"); // 设置狗的名字
  myDog.age = 5;                                     // 设置狗的年龄
  myDog.sex = 'M';                                   // 设置狗的性别
  myDog.hobby = bark;                                // 设置狗的爱好函数

  // 打印狗的信息
  printf("狗的名字是: %s\n", myDog.name);
  printf("狗的年龄是: %d\n", myDog.age);
  printf("狗的性别是: %c\n", myDog.sex);

  // 调用狗的爱好函数
  myDog.hobby();

  return 0;
}
