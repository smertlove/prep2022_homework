#include "utils.h"

size_t timer_from(int from) {
    size_t counter = 0;
    for (int i = from; i >= 0; --i) {
        ++counter;
        printf("%d", i);
        if (i > 0) {
          printf(" ");
        } else {
          printf("\n");
        }
    }
    return counter;
}


int custom_pow(int base, int power) {
  int answ = 1;
  for (; power > 0; power--) {
    answ *= base;
  }
  return answ;
}
