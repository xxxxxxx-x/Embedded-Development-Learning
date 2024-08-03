/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0712/demo2.c
 * @brief        :
 *                 小练习：利用双分支控制流，实现对用户输入的三个数据进行从小到大输出
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-12 14:37:14
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
  int number1, number2, number3;

  // 提示用户输入三个数字，以英文逗号分割
  printf("请任意输入三个数字（以英文逗号分割）："); // 例如输入20,30,10
  scanf("%d,%d,%d", &number1, &number2, &number3);

  // 使用双分支控制流进行排序
  if (number1 > number2) {
    int temp = number1;
    number1 = number2;
    number2 = temp;
  }

  if (number1 > number3) {
    int temp = number1;
    number1 = number3;
    number3 = temp;
  }

  if (number2 > number3) {
    int temp = number2;
    number2 = number3;
    number3 = temp;
  }

  // 从小到大输出排序后的数字
  printf("从小到大输出数字：%d->%d->%d\n", number1, number2, number3);
  return 0;
}
