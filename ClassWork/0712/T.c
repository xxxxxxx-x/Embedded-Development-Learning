#include <stdio.h>

int main(int argc, const char *argv[]) {
  unsigned char ch;
  while (ch = getchar() == 'A')
    printf("%hhu", ch);

  return 0;
}