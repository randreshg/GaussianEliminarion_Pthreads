/***************************LIBRARY DEFINITION**********************************/
#include "utils/utils.h"
#include <sys/time.h>
using namespace std;
/**************************COMPILATION INSTRUCTION*****************************/
/*
1) Create object:
	g++ -pthread utils/utils.cpp gaussian.cpp -o gaussian
2) Run object<
	./gaussian matrixSize NumThreads
	Example: ./gaussian 10 1
*/

/******************************GLOBAL VARIABLES*******************************/
int numThreads = 1;
int size = 5;
int rowItr= 0;
pthread_barrier_t barrier;

/*******************************FUNCTIONS*************************************/
void *gaussianElimination(void *arg);
void parseArguments(int argc, char *argv[]);

/*****************************MAIN FUNCTION***********************************/
int main(int argc, char *argv[]) { 
    parseArguments(argc, argv);
    float **matrix = createMatrix(size);
    cout<<"--------MATRIX--------"<<endl;    
    printMatrix(matrix, size);
    
	//Initiate the time	
	struct timeval t0, t1, dt;
    gettimeofday(&t0, NULL);
    //Init barrier and mutex
    pthread_barrier_init(&barrier, NULL, numThreads);
    // For portability, explicitly create threads in a joinable state 
    pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);    
    //Assign range of rows to each thread and create it
    pthread_t callThd[numThreads];
    int offset = size/numThreads;
    ROWS *rows;        
    for(int i=0; i<numThreads; i++){
        rows = new ROWS;
        rows->thread = i;
        rows->start = i*offset;        
        rows->end = (i+1)*offset;
        rows->matrix = matrix;
        if(rows->end > size)
            rows->end = size;
            cout<<"Thread: "<<i<<"| "<<rows->start<<"-"<<rows->end<<endl;
        pthread_create(&callThd[i], &attr, gaussianElimination, (void *)rows);    
    }
    // Join threads 
    for (int t = 0; t < numThreads; t++)
        pthread_join(callThd[t], NULL);  
    // Clean up and exit 
	pthread_attr_destroy(&attr);	
    pthread_barrier_destroy(&barrier);     
	//Calculate time taken
	gettimeofday(&t1, NULL);
    timersub(&t1, &t0, &dt);	
    cout<<"--------SOLUTION--------"<<endl;
    printMatrix(matrix, size);         
    fprintf(stderr, "The process took %ld.%06ld seconds to execute\n", dt.tv_sec, dt.tv_usec);
    pthread_exit(NULL); 
}

/*******************************FUNCTIONS*************************************/
/* Gaussian elimination */
void *gaussianElimination(void *arg){
    ROWS *rows = (ROWS*) arg;
    float **matrix = rows->matrix;
    float factor1, factor2;    
    while(rowItr<size){        
        if(rowItr >= rows->start && rowItr<rows->end){                        
            factor1 = *(*(matrix + rowItr) + rowItr);            
            //Divide all columns by the factor
            for(int j = rowItr+1; j<size+1; j++){
                *(*(matrix + rowItr) + j) = *(*(matrix + rowItr) + j)/factor1;                
            }
            *(*(matrix + rowItr) + rowItr) = 1;            
        }                        
        //All threads must wait here before continuing        
        pthread_barrier_wait(&barrier);        
        //Loop over the rest of the rows to eliminate the elements
        if(rowItr < size - 1){            
            int row = (rowItr+1> rows->start) ? rowItr+1:rows->start;
            for(row; row<rows->end; row++){            
                factor2 = *(*(matrix + row) + rowItr);
                *(*(matrix + row) + rowItr) = 0;
                // Subtract from each element of the row
                for(int column = rowItr+1; column<size+1; column++){
                    *(*(matrix + row) + column) -= (*(*(matrix + rowItr) + column)) * factor2;
                }
            }
        }
        pthread_barrier_wait(&barrier);
        if(rowItr >= rows->start && rowItr<rows->end)
            rowItr++;
        pthread_barrier_wait(&barrier);
    }    
    pthread_exit((void *)0);
}

/* Parse arguments */
void parseArguments(int argc, char *argv[]){
	if(argc > 1){
		size = atoi(argv[1]);				
		if(argc>2){
			numThreads = atoi(argv[2]);
		} 	
	}    
}