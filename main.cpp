#include <iostream>
#include "mpi.h"


int main(int argc,char *argv[])
{
    double start, stop;
    int i, j, k, l;
    int *A, *B, *C, *buffer, *ans;
    int size = 1000;
    int rank, numprocs, line;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

    line = size/numprocs;
    A = new int[size*size];
    B = new int[size*size];
    C = new int[size*size];

    buffer = new int[size*line];
    ans = new int[size*line];

    if (rank == 0)
    {
        for(i = 0; i < size; i++)
            for(j = 0; j < size; j++)
                A[i * size + j] = (i - 1) * (j + 1);

        for(i = 0; i < size; i++)
            for(j = 0; j < size; j++)
                B[i * size + j] = (i - 2)  * (j + 2);

        start = MPI_Wtime();

        for (i = 1; i < numprocs; i++) {
            MPI_Send(B,size*size,MPI_INT,i,0,MPI_COMM_WORLD);
        }

        for (l = 1; l < numprocs; l++) {
            MPI_Send(A+(l-1)*line*size,size*line,MPI_INT,l,1,MPI_COMM_WORLD);
        }

        for (k = 1; k < numprocs; k++) {
            MPI_Recv(ans,line*size,MPI_INT,k,3,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

            for (i = 0; i < line; i++) {
                for (j = 0; j < size; j++) {
                    C[((k-1)*line+i)*size+j] = ans[i*size+j];
                }
            }
        }

        for (i = (numprocs-1) * line;i < size; i++) {
            for (j = 0; j < size; j++) {
                int temp=0;
                for (k = 0; k < size; k++)
                    temp += A[i*size+k]*B[k*size+j];
                C[i*size+j] = temp;
            }
        }

        stop = MPI_Wtime();

        std::cout << "Time: " << stop-start << "s" << std::endl;



        delete [] A;
        delete [] B;
        delete [] C;
        delete [] buffer;
        delete [] ans;
    }

    else {

        MPI_Recv(B,size*size,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        MPI_Recv(buffer,size*line,MPI_INT,0,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

        for (i = 0; i < line; i++) {
            for (j = 0; j < size; j++) {
                int temp=0;
                for(k = 0; k < size; k++)
                    temp += buffer[i*size+k]*B[k*size+j];
                ans[i*size+j]=temp;
            }
        }

        MPI_Send(ans,line*size,MPI_INT,0,3,MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}