/*******************************************************************************************************
 * @file         : demo23.c
 * @brief        : 输出国际象棋棋盘，棋盘由64个黑白相间的格子组成，分为8行*8列。
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-16 14:29:48
 * @version      : 1.0
 * @note         : 本代码使用嵌套循环控制行和列，根据(i +
 *j)的和的奇偶性来控制输出黑方格或白方格。
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *******************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
  // 遍历8行
  for (int i = 0; i < 8; i++) {
    // 遍历8列
    for (int j = 0; j < 8; j++) {
      // 根据(i + j)的和的奇偶性来决定输出黑方格还是白方格
      if ((i + j) % 2 == 0) {
        printf("⬜"); // 输出白方格
      } else {
        printf("⬛"); // 输出黑方格
      }
    }
    printf("\n"); // 每行结束后换行
  }
  return 0;
}
