/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0715/demo6.c
 * @brief        :
 *                 小练习：编写一个程序，用户输入某个大写字母，产生一个金字塔图案。例如用户输入字母E，则产生如下图案:
 *                                        A
 *                                       ABA
 *                                      ABCBA
 *                                     ABCDCBA
 *                                    ABCDEDCBA
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-15 11:59:31
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <stdio.h>

int main(int argc, const char *argv[]) {
  char input_char;

  // 提示用户输入大写字母
  printf("请输入一个大写字母：");
  scanf("%c", &input_char);

  // 计算金字塔的行数
  int num_rows = input_char - 'A' + 1;

  // 循环打印金字塔每一行
  for (char row_char = 'A'; row_char <= input_char; row_char++) {

    // 打印前导空格
    for (int space = 0; space < num_rows - (row_char - 'A') - 1; space++) {
      printf(" ");
    }

    // 打印升序字符
    for (char ch = 'A'; ch <= row_char; ch++) {
      printf("%c", ch);
    }

    // 打印降序字符（不包括中间的字符）
    for (char ch = row_char - 1; ch >= 'A'; ch--) {
      printf("%c", ch);
    }

    // 每行结束后换行
    printf("\n");
  }

  return 0;
}
