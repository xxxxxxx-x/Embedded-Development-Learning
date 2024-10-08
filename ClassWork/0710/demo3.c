/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0710/demo3.c
 * @brief        : 小练习：获取用户输入的长宽高，计算长方体的面积和体积并输出
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-10 15:53:15
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2023-2024   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>
int main(int argc, const char *argv[]) {
  double x, y, z;
  printf("请输入长度：");
  scanf("%lf", &x);
  printf("请输入宽度：");
  scanf("%lf", &y);
  printf("请输入高度：");
  scanf("%lf", &z);
  printf("这个长方体的面积是%.1lf,体积是%.1lf\n", (x * y + x * z + y * z) * 2,
         x * y * z);
  return 0;
}