/*
* Student: Desmond Stular
* Class: CMPT 360
* Lab 07: Threads
*/

#include "matrixLibrary.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int nt = atoi(argv[2]);
    int n2 = n*n;
    int *m1, *m2, *m3;
    int a=2, b=5;
    int sum;

    // Generate the matrices
    m1 = malloc(n2*sizeof(int));
    m2 = malloc(n2*sizeof(int));
    m3 = malloc(n2*sizeof(int));

    generateMatrix(m1, n);
    generateMatrix(m2, n);
    matrixCopy(m1, m2, &n, &nt);
    matrixSaxpy(m2, m3, &a, &b, &n, &nt);
    printf("Matrix 2\n");
    printMatrix(m2, n);
    printf("\nMatrix 3\n");
    printMatrix(m3, n);

    printf("\nTesting innerproduct on matrix 2 and 3\n");

    innerProduct(&sum, m2, m3, &n2, &nt);

    printf("sum = %d\n", sum);

    free(m1);
    free(m2);
    free(m3);

    exit(0);
}
