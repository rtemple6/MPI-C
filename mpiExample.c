//Sending and receiving message

//Compile
//mpicc -o name filename.c
//Execute
//mpiexec -n (num of process) ./name arg1 arg2

#include <stdio.h>
#include <string.h>
#include <mpich/mpi.h>

const int MAX_STRING = 100;

int main(int argc, char *argv[])
{
    int nummsg = 10;
    //Buffer message which we will send
    char msg[MAX_STRING];
    //Number of nodes/processes
    int comm_sz;
    //Process Rank/process-id
    int my_rank;
    
    //Start MPI
    MPI_Init(&argc, &argv);

    //MPI_COMM_WORLD lets you know if belonds to same cluster
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank != 0) {
        //Data node
        sprintf(msg, "Hello World from process %d of %d !", my_rank, comm_sz);

        for(int i = 0; i < nummsg; i++)
        {
            //Send Message
            MPI_Send(msg, strlen(msg)+1, MPI_CHAR, 0, i, MPI_COMM_WORLD);
        }
        
    } else {
        //Recieve Messages
        for(int i = 1; i < comm_sz; i++)
        {
            for(int j = 0; j < nummsg; j++)
            {
                /* code */
                MPI_Recv(msg, MAX_STRING, MPI_CHAR, i, j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("%s Tag: %d\n", msg,j);
            }
        }
    }

    MPI_Finalize();

    return 0;
}
