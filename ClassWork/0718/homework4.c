/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0718/homework4.c
 * @brief        : 作业：下列语句中哪些是正确的，哪些是错误的？原因是什么？
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-18 21:06:22
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

int main(int argc, const char *argv[]) {

  float apple[10], apple_tree[10][5], *pf, weight = 2.2;
  int i = 3;

  // a. apple[2] = weight;
  // 合法：这是一个普通的数组赋值操作，将weight的值赋给apple数组的第3个元素。

  // b. scanf("%f", &apple);
  // 非法：scanf("%f",
  // &apple)是错误的，应该使用&apple[index]或者apple来表示数组的地址，但&apple会导致警告。

  // c. apple = weight;
  // 非法：apple是数组名，表示的是数组的首地址，是一个常量，不可赋值。

  // d. printf("%f", apple[3]);
  // 合法：打印apple数组的第4个元素的值。

  // e. apple_tree[4][4] = apple[3];
  // 合法：将apple数组的第4个元素的值赋给apple_tree的第5行第5列的元素。

  // f. apple_tree[5] = apple;
  // 非法：apple_tree[5]是一个float[5]类型的数组，而apple是一个float[10]类型的数组，两者类型不匹配，不能直接赋值。

  // g. pf = weight;
  // 非法：pf是一个float指针，应该赋值一个地址，而weight是一个float值。

  // h. pf = apple;
  // 合法：apple是一个float数组名，表示数组的首地址，pf是一个float指针，可以指向该地址。

  return 0;
}
