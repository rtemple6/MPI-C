#include <stdio.h>
#include <string.h>
#include <mpich/mpi.h>

double f(double x);
//Trap(A,B,,H)
double Trap(double left_endpt, double right_endpt, int trap_count, double base_len);

int main(int argc, char *argv[])
{
    //n is num of segments
    int my_rank, comm_sz, n=1024, local_n;
    double a=0, b=3.0, h, local_a, local_b;
    double local_int, total_int;
    int source;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    //Get height of trapezoid
    h = (b-a)/n;

    //Num of trapezoids assign to each data note
    local_n = n/comm_sz;

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    local_int = Trap(local_a, local_b, local_n, h);

    if(my_rank != 0){
        MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }else{
        total_int = local_int;
        
        for(source = 1; source < comm_sz; source++) {
            MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            //Collect full integratin
            total_int = total_int + local_int;
        }
        
    }

    
    if (my_rank == 0) {
        printf("With n=%d our estimate\n", n);
        printf("of the integral from %f to %f = %.15e\n", a,b,total_int);
    }
    
    MPI_Finalize();
    return 0;
}

double f(double x) {
    return x*x*x;
}

double Trap(double left_endpt, double right_endpt, int trap_count, double base_len){ 
    double estimate, x;
    int i;
    estimate = (f(left_endpt)+f(right_endpt)) / 2;
    
    for( i = 1; i < trap_count; i++)
    {
        x = left_endpt + i * base_len;
        estimate += f(x);
    }
    estimate = estimate * base_len;
    return estimate;
}