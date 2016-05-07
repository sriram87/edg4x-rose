#include "mpi.h"

int foo(int x) {
  return x+1;
}

int main() {
  int x, y = 0;
  x = foo(y);
  if(x) {
    y = foo(x);
  }
  else {
    x = foo(y);
  }
  return x+y;
}
