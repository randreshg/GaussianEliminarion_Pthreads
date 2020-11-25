#include "utils.h"
using namespace std;

float** createMatrix(int size){
    float **matrix = new float*[size];
    for(int i=0; i<size; i++)
        *(matrix + i) = new float[size+1];
    fillMatrix(matrix, size);
    return matrix; 
}

void fillMatrix(float **matrix, int size){
    srand( (unsigned)time(NULL) );
    for(int i=0; i<size; i++){
        for(int j=0; j<size+1; j++){
            *(*(matrix + i) + j) = rand()%100+1;
        }
    }
}

void printMatrix(float **matrix, int size){
    float value = 0;
    for(int i=0; i<size; i++){
        for(int j=0; j<size+1; j++){
            value = roundf(*(*(matrix + i) + j) * 100) / 100;
            if(j == size)
                cout << right << setw(10) << "| " << value;
            else           
                cout << right << setw(10) << value;
        }
        cout<<endl<<endl;
    }
}