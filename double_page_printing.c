// the stupid CUPS driver doesn't wait for me
// to put the paper in the printer, so print
// the odd pages first, then the even pages.

#include <stdio.h>

int main() {
  unsigned total;

  printf("total: ");
  scanf("%d", &total);

  if (total <= 1) {
    printf("no");
    return 1;
  }

  printf("1");
  for (int i = 3; i <= total; i += 2) {
    printf(",%d", i);
  }
  printf("\n2");
  for (int i = 4; i <= total; i += 2) {
    printf(",%d", i);
  }
  printf("\n");

  return 0;
}
