#include "mpi.h"

int main(int argc, char* argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int msg;
  if(rank == 0) {
    msg = 1;
    MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    msg = 2;
    MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  }
  else if(rank == 1) {
    MPI_Status status;
    MPI_Recv(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  msg = msg + 1;
  MPI_Finalize();
  return msg;
}
