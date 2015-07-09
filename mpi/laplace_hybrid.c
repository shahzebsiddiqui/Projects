/****************************************************************
 * Laplace MPI Template C Version                                         
 *                                                               
 * T is initially 0.0                                            
 * Boundaries are as follows                                     
 *                                                               
 *                T                      4 sub-grids            
 *   0  +-------------------+  0    +-------------------+       
 *      |                   |       |                   |           
 *      |                   |       |-------------------|         
 *      |                   |       |                   |      
 *   T  |                   |  T    |-------------------|             
 *      |                   |       |                   |     
 *      |                   |       |-------------------|            
 *      |                   |       |                   |   
 *   0  +-------------------+ 100   +-------------------+         
 *      0         T       100                                    
 *                                                                 
 * Each PE only has a local subgrid.
 * Each PE works on a sub grid and then sends         
 * its boundaries to neighbors.
 *                                                                 
 *  John Urbanic, PSC 2014
 * 
 *  Developer: Shahzeb Siddiqui
 *
 *******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <mpi.h>

#define ceil(x,y) (((x) + (y) -1) / (y))

#define COLUMNS      10000
#define ROWS_GLOBAL  10000        // this is a "global" row count
#define NPES         10

#if ROWS_GLOBAL % NPES == 0
#define ROWS ROWS_GLOBAL/NPES   // number of real local rows
#else
#define ROWS (ROWS_GLOBAL/NPES + 1)   // number of real local rows
#endif

// communication tags
#define DOWN     100
#define UP       101   

#define MAX_TEMP_ERROR 0.01
double Temperature[ROWS+2][COLUMNS+2];
double Temperature_last[ROWS+2][COLUMNS+2];

void initialize(int npes, int my_PE_num);
void track_progress(int iter,int my_PE_num, int row_offset);
int start = -1, end = -1,chunk = -1;

int main(int argc, char *argv[]) {

    int i, j;
    int pid, row_offset;
    int max_iterations;
    int iteration=1;
    double dt;
    struct timeval start_time, stop_time, elapsed_time;
    int row_sol;
    int        npes;                // number of PEs
    int        my_PE_num;           // my PE number
    double     dt_global=100;       // delta t across all PEs
    MPI_Status status;              // status returned by MPI calls
    MPI_Request request;

    // the usual MPI startup routines
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_PE_num);
    MPI_Comm_size(MPI_COMM_WORLD,&npes);
 
// verify only NPES PEs are being used 
    if (npes != NPES)
    {
	if (my_PE_num == 0)
		printf("MPI Program using %d Processes, application must run with %d Processes\n",npes,NPES);
	exit(1);
    }
   
    start = my_PE_num*ROWS+1;
    end = (my_PE_num+1)*ROWS;

    // for Last Proc, make end value ROWS_GLOBAL as last row 
    if (end > ROWS_GLOBAL)
      end = ROWS_GLOBAL;
  
    chunk = end - start;
 
 
    for (i = 0 ; i < NPES; i++)
    {

      MPI_Barrier(MPI_COMM_WORLD);
      if (my_PE_num == i)
        printf("Proc: %d \t start: %d \t end: %d chunk: %d \n", my_PE_num, start, end, chunk);

    }

    MPI_Barrier(MPI_COMM_WORLD);
//    exit(1);

    // calculating ROW 7500 with different process configuration for printing Temperature[7500][9950]
    row_sol = 7500;
    if (start <= row_sol && row_sol <= end)
    {
	pid = my_PE_num;
        row_offset = row_sol - start;    
    }
   
    if (my_PE_num == pid)
    {
	printf("Proc %d will print Temperature[7500][9950]\n", pid);
	printf("Offset: %d\n",row_offset);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // PE 0 asks for input
    if (my_PE_num == 0)
    {
	printf("[Max iterations 100-4000]?\n");
	scanf("%d", &max_iterations);
    }

    // bcast max iterations to other PEs
    MPI_Bcast(&max_iterations,1,MPI_INT, 0,MPI_COMM_WORLD);
    if (my_PE_num==0) gettimeofday(&start_time,NULL);

    initialize(npes, my_PE_num);
    
    #pragma acc enter data copyin(Temperature_last), create(Temperature)
    while ( dt_global > MAX_TEMP_ERROR && iteration <= max_iterations ) {
	dt = 0.0;
	dt_global = 0.0;
        // main calculation: average my four neighbors
	#pragma acc kernels present(Temperature,Temperature_last) 
	#pragma acc loop gang vector(1)
        for(i = 1; i <= chunk; i++) {
	    #pragma acc loop gang vector(128)
            for(j = 1; j <= COLUMNS; j++) {
                Temperature[i][j] = 0.25 * (Temperature_last[i+1][j] + Temperature_last[i-1][j] +
                                            Temperature_last[i][j+1] + Temperature_last[i][j-1]);

	        dt = fmax( fabs(Temperature[i][j]-Temperature_last[i][j]), dt);
            }
        }
       MPI_Allreduce(&dt,&dt_global,1,MPI_DOUBLE,MPI_MAX,MPI_COMM_WORLD);
	#pragma acc kernels present(Temperature_last, Temperature)
	#pragma acc loop gang vector(1)
	for (i = 1; i <= chunk; i++)
	{
	  #pragma acc loop gang vector(128)
	  for (j = 1; j <= COLUMNS; j++)
	  {
	    Temperature_last[i][j] = Temperature[i][j]; 
	  }
	}

        // COMMUNICATION PHASE: send and receive ghost rows for next iteration
	 MPI_Request two_way_req[4];	  // Processes with two way halo exchange include Proc rank [1,NPES-2} use this request array for Asynchronous Communication
	 MPI_Request one_way_req[2];	 // Processes with one way halo exchange include Proc rank 0,NPES-1 use this request array for Asynchronous Communication
	 MPI_Status two_way_req_stat[4];
 	 MPI_Status one_way_req_stat[2];
	
	#pragma acc host_data use_device(Temperature_last)
	 {
	   // send ghost cell down
	   if (my_PE_num != npes-1)
           {
	     if (my_PE_num != 0)
	        MPI_Isend(&Temperature_last[chunk][1],COLUMNS,MPI_DOUBLE,my_PE_num+1,DOWN,MPI_COMM_WORLD,&two_way_req[0]);
	     else
	        MPI_Isend(&Temperature_last[chunk][1],COLUMNS,MPI_DOUBLE,my_PE_num+1,DOWN,MPI_COMM_WORLD,&one_way_req[0]);
	   }
	   if (my_PE_num != 0)
	   {
             if (my_PE_num != npes-1)	
	       MPI_Irecv(&Temperature_last[0][1],COLUMNS,MPI_DOUBLE,my_PE_num-1,DOWN,MPI_COMM_WORLD,&two_way_req[1]);
	     // last process first recieve last row from process npes-2 with Req index of 0 
	     else
  	       MPI_Irecv(&Temperature_last[0][1],COLUMNS,MPI_DOUBLE,my_PE_num-1,DOWN,MPI_COMM_WORLD,&one_way_req[0]);
           }
	   // send ghost cell up
	   if (my_PE_num != 0)
	   {
	      if(my_PE_num != npes-1)
                MPI_Isend(&Temperature_last[1][1], COLUMNS, MPI_DOUBLE, my_PE_num-1, UP, MPI_COMM_WORLD,&two_way_req[2]);
	      // last process only has 1 Isend,Irecv with its npes-2 proc so request array index is 1, Req Index 0 was done in above MPI_Irecv
	      else
                MPI_Isend(&Temperature_last[1][1], COLUMNS, MPI_DOUBLE, my_PE_num-1, UP, MPI_COMM_WORLD,&one_way_req[1]);
	   }
	   if (my_PE_num != npes-1)
	   {
	      if (my_PE_num != 0)
                MPI_Irecv(&Temperature_last[chunk+1][1],COLUMNS,MPI_DOUBLE,my_PE_num+1,UP,MPI_COMM_WORLD,&two_way_req[3]);
	      // Proc 0 only has one Isend,Irecv statement, so request array index is 0
	      else
                MPI_Irecv(&Temperature_last[chunk+1][1],COLUMNS,MPI_DOUBLE,my_PE_num+1,UP,MPI_COMM_WORLD,&one_way_req[1]);
	   }
	}


	// wait for all MPI_Isend and MPI_Irecv for halo exchange
	if (my_PE_num != 0 && my_PE_num != npes-1)
  	  MPI_Waitall(4,two_way_req,two_way_req_stat);
	else
  	  MPI_Waitall(2,one_way_req,one_way_req_stat);
	

        if((iteration % 100) == 0) {
            if (my_PE_num == pid){
		printf("Iteration: %d \t dt_global: %f\n",iteration, dt_global);
//		#pragma acc update host(Temperature[:][COLUMNS-50:10])
//                 track_progress(iteration,my_PE_num, row_offset);
		//}
	    }
        }
	iteration++;
    }
// only copyout value of Temperature from Process that has row 7500 to minimize memory transfer and since we care only for value of Temperature[7500][9950] for accuracy
if (my_PE_num == pid)
{
    #pragma acc exit data copyout(Temperature) delete(Temperature,Temperature_last)
}
else
{
    #pragma acc exit data delete(Temperature,Temperature_last)
}
    // Slightly more accurate timing and cleaner output 
    MPI_Barrier(MPI_COMM_WORLD);

    // PE 0 finish timing and output values
    if (my_PE_num==0){
        gettimeofday(&stop_time,NULL);
	timersub(&stop_time, &start_time, &elapsed_time);


	printf("\nMax error at iteration %d was %f\n", iteration-1, dt_global);
	printf("Total time was %f seconds.\n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);
     }

    MPI_Barrier(MPI_COMM_WORLD);
    if (my_PE_num == pid)
    {
       track_progress(iteration,my_PE_num,row_offset);
    }

    MPI_Finalize();
}



void initialize(int npes, int my_PE_num){

    double tMin, tMax;  //Local boundary limits
    int i,j;
    #pragma omp parallel for private(i,j) shared(Temperature_last) schedule(dynamic)
    for(i = 0; i <= chunk+1; i++){
        for (j = 0; j <= COLUMNS+1; j++){
            Temperature_last[i][j] = 0.0;
        }
    }

    // Local boundry condition endpoints
    tMin = (my_PE_num)*100.0/npes;
    tMax = (my_PE_num+1)*100.0/npes;

    #pragma omp parallel for private(i,j) shared(Temperature_last) schedule(dynamic)
    for(i = 0; i <= chunk+1; i++)
    {
	Temperature_last[i][0]=0.0;
	Temperature_last[i][COLUMNS+1]= (start + i)/(100.0);
//	printf("Proc %d \t Temperature_last[%d][%d]: %f\n",my_PE_num,i,COLUMNS+1,Temperature_last[i][COLUMNS+1]);
    }
    // first row boundary condition for Proc 0
    if (my_PE_num == 0)
      #pragma omp parallel for private(j) shared(Temperature_last) schedule(dynamic)
      for (j = 0; j <= COLUMNS+1; j++)
	 Temperature_last[0][j] = 0.0;
    // last row boundary condition for last Proc
    if (my_PE_num == npes-1)
      #pragma omp parallel for private(j) shared(Temperature_last) schedule(dynamic)	
      for (j = 0; j <= COLUMNS+1; j++)
	{
	Temperature_last[chunk+1][j] = (100.0/COLUMNS)*j;		
	//printf("Proc %d \t Temperature_last[%d][%d]: %f\n",my_PE_num,j,chunk+1,Temperature_last[chunk+1][j]);
	}
}


// only called by last PE
void track_progress(int iteration,int rank, int row_offset) {

    int i,j;
    printf("---------- Iteration number: %d ------------\n", iteration);
    for (j = 50; j >= 45; j--){
      printf("[%d,%d]: %5.4f ", 7500 , COLUMNS-j, Temperature[row_offset][COLUMNS-j]);

//      printf("[%d,%d]: %5.4f ", ROWS_GLOBAL-j , COLUMNS-j, Temperature[ROWS-j][COLUMNS-j]);
    }
    printf("\n");
}
