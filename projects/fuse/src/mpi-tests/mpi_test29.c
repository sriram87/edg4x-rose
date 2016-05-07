#include "_mpi.h"
//#include <stdio.h>

int CompDebugAssert(int expr);

//#pragma fuse seq(mcc, pt, cp, mv, cp, dp, cp, mco, cp, mco, cp)
#pragma fuse seq(mcc, pt, cp, mv, cp, dp, cp, mdv, mco, mdv)

int main(int argc, char* argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int x, y, ln, rn;
  MPI_Status status;

  if(rank != 0) ln = rank-1;    
  else ln = size-1;

  if(rank != size-1) rn = rank+1;
  else rn = 0;

  //printf("rank=%d, ln=%d, rn=%d\n", rank, ln, rn);
  
  if(rank % 2 == 0) {
    x = 5;
    MPI_Send(&x, 1, MPI_INT, rn, 0, MPI_COMM_WORLD);
    MPI_Recv(&y, 1, MPI_INT, ln, 0, MPI_COMM_WORLD, &status);
  }
  else  {
    MPI_Recv(&x, 1, MPI_INT, ln, 0, MPI_COMM_WORLD, &status);
    y = 2 * x;
    MPI_Send(&y, 1, MPI_INT, rn, 0, MPI_COMM_WORLD);
  }

  //printf("x=%d, y=%d\n", x, y);
  CompDebugAssert(y == 2*x);
  MPI_Finalize();
  return 0;
}
