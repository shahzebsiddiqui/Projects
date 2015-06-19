// Shahzeb Siddiqui
// Implementation of Game Of Life by James Conway
// Project Description: http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main (int argc, char* argv[])
{
  int array_size;
  int **array2d_old;
  int **array2d_new;
  // these four arrays are used for storing the left,right,top, and bottom boundary with ghost cells for computing adjacent values at border. 
  int *left_ghost;
  int *right_ghost;
  int *top_ghost;
  int *bottom_ghost;
  int num_live_cells = 0;
  int t_step = 10; // t_step is the total time iterations in game of life
  int i, j;
  int t = 0;		  // t is time variable used to indicate the nth generation in game of life
  if (argc != 2)
  {
	  printf("usage error: provide array size as input to parameter. Format <executable> <array_size> where array_size is integer \n");
	  exit(1);
  }	  
  else
    array_size = atoi(argv[1]);
  
  
  array2d_old = (int**)malloc(sizeof(int*)*array_size);
  array2d_new = (int**)malloc(sizeof(int*)*array_size);
  
  // allocate memory for ghost arrays
  left_ghost = (int*)malloc(sizeof(int)*array_size);
  right_ghost = (int*)malloc(sizeof(int)*array_size);
  top_ghost = (int*)malloc(sizeof(int)*array_size);
  bottom_ghost = (int*)malloc(sizeof(int)*array_size);
  // allocate memory for 2D array for old and new array which will be used in updating data across time iteration
  for (i = 0; i < array_size; i++)
  {
    array2d_old[i] = (int*)malloc(sizeof(int)*array_size);
	array2d_new[i] = (int*)malloc(sizeof(int)*array_size);
  }
  srand(time(NULL));
  // randomize input data either 0 or 1, 0 indicates dead cell, 1 indicates live cell
  for (i = 0; i < array_size; i++)
  {
    for (j = 0; j < array_size; j++)
	{
	  int num = rand() % 100;
	  if (num < 50)
		array2d_old[i][j] = 0;
	  else
		array2d_old[i][j] = 1;
	}
  }
    
  for (i = 0; i < array_size; i++)
  {
  // copy array's last column into left ghost
    left_ghost[i] = array2d_old[i][array_size-1];
	  
    //copy array first column into right ghost
	right_ghost[i] = array2d_old[i][0];	 	
	
	//copy last row into top ghost
	top_ghost[i] = array2d_old[array_size-1][i];
	
	//copy first row into bottom ghost
	bottom_ghost[i] = array2d_old[0][i];
  }
  printf("Initial Input:\n");
  print2DArray(array2d_old,array_size); 
  printf("Left Ghost:");
  print1DArray(left_ghost,array_size);  	
  printf("Right Ghost:");
  print1DArray(right_ghost,array_size);
  printf("Top Ghost:");
  print1DArray(top_ghost,array_size);
  printf("Bottom Ghost:");
  print1DArray(bottom_ghost,array_size);
  
  printf("----------------------------------------------------\n");
  // four variables to store the four neighbours
  
  // main loop for Game Of Life - number of time iterations to run for input data
  while (t < t_step)
  {
	  for (i = 0; i < array_size; i++)
	  {
		for (j = 0; j < array_size; j++)	
		{
		  int top,bottom,left,right,topleft,topright,bottomleft,bottomright;    
		  int neighbor_live_cell_cnt = 0;
		  
		  
		  // top left corner use left and top ghost cell
		  if (i == 0  && j == 0)
		  {
			left = left_ghost[0];
			top = top_ghost[0];
			
			right = array2d_old[i][j+1];
			bottom = array2d_old[i+1][j];
			
			topleft = top_ghost[array_size-1];
			topright = top_ghost[i+1];
			bottomleft = left_ghost[i+1];
			bottomright = array2d_old[i+1][j+1];
		  }
		  // top right corner use right and top ghost cell
		  else if(i == 0 && j == array_size-1)
		  {
			right = right_ghost[0];
			top = top_ghost[array_size-1];
			
			left = array2d_old[i][j-1];
			bottom = array2d_old[i+1][j];
			
			topleft = top_ghost[array_size-2];
			topright = top_ghost[0];
			bottomleft = array2d_old[i+1][j-1];
			bottomright = right_ghost[i+1];
		  }
		  // bottom left corner uses left and bottom ghost cell
		  else if(i == array_size-1 && j == 0)
		  {
		    left = left_ghost[array_size-1];
			bottom = bottom_ghost[0];
			
			right = array2d_old[i][j+1];
			top = array2d_old[i-1][j];
			
			topleft = top_ghost[array_size-2];
			topright = array2d_old[i-1][j+1];
			bottomleft = left_ghost[0];
			bottomright = bottom_ghost[j+1];
		  }
		  // bottom right corner uses right and bottom ghost cell
		  else if(i == array_size-1 && j == array_size-1)
		  {
		    right = right_ghost[array_size-1];
			bottom = bottom_ghost[array_size-1];
			
			left = array2d_old[i][j-1];
			top = array2d_old[i-1][j];
			
			topleft = array2d_old[i-1][j-1];
			topright = right_ghost[array_size-2];
			bottomleft = bottom_ghost[array_size-2];
			bottomright = bottom_ghost[0];
		  }
		  // use top ghost
		  else if(i == 0 && (j != 0 || j != array_size-1))
		  {
		    top = top_ghost[j];
			
		    left = array2d_old[i][j-1];			
			right = array2d_old[i][j+1];			
			bottom = array2d_old[i+1][j];
			
			topleft = top_ghost[j-1];
			topright = top_ghost[j+1];
			bottomleft = array2d_old[i+1][j-1];
			bottomright = array2d_old[i+1][j+1];
		  }
		  // use bottom ghost
		  else if(i == array_size-1 && (j != 0 || j != array_size-1))
		  {
		    bottom = bottom_ghost[j];
			
			left = array2d_old[i][j-1];
			right = array2d_old[i][j+1];			
			top = array2d_old[i-1][j];			

			topleft = array2d_old[i-1][j-1];
			topright = array2d_old[i-1][j+1];
			bottomleft = bottom_ghost[j-1];
			bottomright = bottom_ghost[j+1];
		  }
		  // use left ghost
		  else if(j == 0 && (i != 0 || i != array_size-1))
		  {
		    left = left_ghost[i];
			
			right = array2d_old[i][j+1];			
			top = array2d_old[i-1][j];					
			bottom = array2d_old[i+1][j];
			
			topleft = left_ghost[i-1];
			topright = array2d_old[i-1][j+1];
			bottomleft = left_ghost[i+1];
			bottomright = array2d_old[i+1][j+1];
		  }
		  // use right ghost
		  else if(j == array_size-1 && (i != 0 || i != array_size-1))
		  {
		    right = right_ghost[i];
			
			left = array2d_old[i][j-1];
			top = array2d_old[i-1][j];					
			bottom = array2d_old[i+1][j];
			
			topleft = array2d_old[i-1][j-1];
			topright = right_ghost[i-1];
			bottomleft = array2d_old[i+1][j-1];
			bottomright = right_ghost[i+1];
		  }
		  // no ghost cells needed
		  else
		  {
			  top = array2d_old[i-1][j];
			  bottom = array2d_old[i+1][j];
			  left = array2d_old[i][j-1];
			  right = array2d_old[i][j+1];
			  
			  topleft = array2d_old[i-1][j-1];
			  topright = array2d_old[i-1][j+1];
			  bottomleft = array2d_old[i+1][j-1];
			  bottomright = array2d_old[i+1][j+1];
		  }
		  // count number of live cells
		  neighbor_live_cell_cnt = top + bottom + left + right + topleft + topright + bottomleft + bottomright;
		  
		  switch (neighbor_live_cell_cnt)
		  {
			// if three live cell neighbours then cell is alive
			case 3: array2d_new[i][j] = 1; break;			
			// if two live cell neighbours then there is no change 
			case 2: array2d_new[i][j] = array2d_old[i][j]; break;
			// all other cases, the cell becomes dead
			default: array2d_new[i][j] = 0; break;
			
		  }
		}
	  }
	  printf("\033[0;37m Time Iteration = %d\n \033[37m", t+1);
	  printf("Array: \n");	  
	  print2DArray(array2d_new,array_size);
	
	  // update old array with new array for next iteration
	  for (i = 0; i < array_size; i++)
	  {
		for (j = 0; j < array_size; j++)
		{
		  array2d_old[i][j] = array2d_new[i][j];
		}
	  }
	  // update ghost cells with updated array
	  for (i = 0; i < array_size; i++)
	  {
	  // copy array's last column into left ghost
		left_ghost[i] = array2d_old[i][array_size-1];
		  
		//copy array first column into right ghost
		right_ghost[i] = array2d_old[i][0];	 	
		
		//copy last row into top ghost
		top_ghost[i] = array2d_old[array_size-1][i];
		
		//copy first row into bottom ghost
		bottom_ghost[i] = array2d_old[0][i];
	  }
	 /*  printf("Left Ghost:");
	  print1DArray(left_ghost,array_size);  	
	  printf("Right Ghost:");
	  print1DArray(right_ghost,array_size);
	  printf("Top Ghost:");
	  print1DArray(top_ghost,array_size);
	  printf("Bottom Ghost:");
	  print1DArray(bottom_ghost,array_size); */
	  printf("\033[0;37m----------------------------------------------------\n \033[37m");
	  
	  t++;
	}
    
	for (i = 0; i < array_size; i++)
	{
	  for (j = 0; j < array_size; j++)
	  {
	    if (array2d_new[i][j] == 1)
		  num_live_cells++;
	  }
	}
	printf("Total Live cells : %d after %d iterations\n", num_live_cells,t_step);
  return 0;
}

void print2DArray(int **arr, int n)
{
  int i,j;
  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
	{
    	if (arr[i][j] == 1)
		  printf("\033[0;32m %d\033[0m ", arr[i][j]);
		else
		  printf("\033[0;31m %d\033[0m ", arr[i][j]);
    }		
	printf("\n");  
  }
}
void print1DArray(int *array, int n)
{
  int i;
  for (i = 0; i < n; i++)
    printf("%d ",array[i]);
  
  printf("\n");	
}
