/*****************************LIBRARY DEFINITION********************************/
#ifndef __UTILS_H__
#define __UTILS_H__

/* Libraries */
#include <pthread.h>
#include <iostream> /* cout */
#include <stdlib.h> /* srand, rand */
#include <math.h> /* roundf */
#include <iomanip>

typedef struct{
    int thread;
	int start;
	int end;
	float **matrix;
} ROWS;

/* Functions */
float **createMatrix(int size);
void fillMatrix(float **matrix, int size);
void printMatrix(float **matrix, int size);
#endif /* __UTILS_H__ */