//#include <stdio.h>
//#include <math.h>
#include "_mpi.h"

/* This example handles a 12 x 12 mesh, on 4 processors only. */
#define maxn 12

int main(int argc, char* argv[] )
{
  double     xlocal[(12/4)+2][12];
  double     xnew[(12/3)+2][12];

  int i, j, i_first=0, i_last=0;
  for (i=i_first; i<=i_last; i++) {
    for (j=1; j<maxn-1; j++) {
      xlocal[i][j] = xnew[i][j];
    }
  }
}
 
