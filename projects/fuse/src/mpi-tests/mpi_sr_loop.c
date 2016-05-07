#include "mpi.h"
//#include <stdio.h>

int left(int rank, int size) {
  if(rank == 0) return size-1;
  else return rank-1;
}

int right(int rank, int size) {
  if(rank == size-1) return 0;
  else return rank+1;
}

int main(int argc, char* argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int msg;
  unsigned int time = 100;
  MPI_Status status;
  while(time) {
    if(rank % 2 == 0) {
      MPI_Send(&rank, 1, MPI_INT, left(rank, size), 0, MPI_COMM_WORLD);
      MPI_Recv(&msg, 1, MPI_INT, right(rank, size), 0, MPI_COMM_WORLD, &status);
    }
    else {
      MPI_Recv(&msg, 1, MPI_INT, right(rank, size), 0, MPI_COMM_WORLD, &status);
      MPI_Send(&rank, 1, MPI_INT, left(rank,size), 0, MPI_COMM_WORLD);
    }
    --time;
  }
  MPI_Barrier(MPI_COMM_WORLD);
  //printf("rank=%d, msg=%d\n", rank, msg);
  return 0;
}
  
