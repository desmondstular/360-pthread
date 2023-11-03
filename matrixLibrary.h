/*
* STUDENT: DESMOND STULAR
* CMPT360 FALL 2023
* LAB7 - THREADS
*/


#ifndef MATRIXLIBRARY_H_
#define MATRIXLIBRARY_H_

int matrixCopy(int* m1, int* m2, int* n, int* nt);

void* tfunMatrixCopy(void* arg);

int matrixSaxpy(int* m1, int* m2, int* a, int* b, int* n, int* nt);

void* tfunMatrixSaxpy(void* arg);

int innerProduct(int* l1, int* v1, int* v2, int* n, int* nt);

void* tfunInnerProduct(void* arg);

void generateMatrix(int *matrix, int n);

void printMatrix(int *matrix, int n);

#endif
