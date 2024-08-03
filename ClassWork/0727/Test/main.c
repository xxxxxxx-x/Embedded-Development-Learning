#include <stdio.h>

int main() {
  unsigned short int i = 0x1234;
  printf("%0#4x\n", *(unsigned char *)&i);

  int *p = (void *)0xa0001234;
  *p = 1;
  //   printf("%0#4x\n", );
}
