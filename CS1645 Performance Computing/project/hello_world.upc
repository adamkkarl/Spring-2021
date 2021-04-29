#include <upc.h>

int main(int argc, char const *argv[]) {
  printf("hello from thread %d\n", MYTHREAD);
  return 0;
}
