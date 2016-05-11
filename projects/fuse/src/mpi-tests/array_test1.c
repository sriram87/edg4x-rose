#include "_mpi.h"

#pragma fuse seq(mcc, cp, arr, pt, mv, cp, dp, mdv, mco, mdv)

int main(/*int argc, char* argv[]*/) {
  int rank, size;
  int array[10][20];
  int i, j;
  i = 0; j = 1;
  //MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(rank == 0)
    MPI_Send(array[i], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  else if (rank == 1) {
    MPI_Status status;
    MPI_Recv(array[j], 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }
  return 0;
}
