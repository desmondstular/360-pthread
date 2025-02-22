/*
* STUDENT: DESMOND STULAR (3067040)
* CMPT360 FALL 2023
* LAB 7 - PTHREADS
*/


#include "matrixLibrary.h"
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <sys/sysinfo.h>

sem_t sem;				// semaphore
int innerProductSum;	//used by innerProduct

typedef struct threadData1 {
    int *m1;
    int *m2;
    int istart;
    int iend;
    int n2;
} threadData1;

typedef struct threadData2 {
    int *m1;
    int *m2;
    int a;
    int b;
    int istart;
    int iend;
    int n2;
} threadData2;

typedef struct threadData3 {
    int *v1;
    int *v2;
    int istart;
    int iend;
    int n;
} threadData3;

int matrixCopy(int* m1, int* m2, int* n, int* nt) {
    clock_t startTime = clock();
	int n2 = (*n)*(*n);
    int perThread = ceil( (double)(n2) / (double)(*nt) );
    int start=0, end=perThread;
    int nthread = 0;
    pthread_t tid[*nt];
    threadData1 targ[*nt];
    // Initiate data for each thread and create
    for (int i=0; i < (*nt); i++) {
        targ[i].m1 = m1;
        targ[i].m2 = m2;
        targ[i].istart = start;
        targ[i].iend = end;
        targ[i].n2 = n2;
        pthread_create(&tid[i], NULL, tfunMatrixCopy, (void*)&targ[i]);

        start += perThread;
        end += perThread;
        // If more threads than indices, cause loop to break
	if (start >= n2) {
	    nthread = i+1;
	    break;
        }
    }

    for (int i=0; i < nthread; i++) {
        pthread_join(tid[i], NULL);
    }
	
	clock_t endTime = clock();
	int timeTook = (double)(endTime - startTime) / CLOCKS_PER_SEC * pow(10, 6);
	return timeTook;
}

void* tfunMatrixCopy(void* arg) {
    threadData1 *data = (threadData1*)arg;
    int j = data->iend;

    // if end exceeds # index, assign end to # index
    if (j >= data->n2) {
        j = data->n2;
    }
    
    // Assign range of indices to second matrix
    for (int i = data->istart; i < j; i++) {
	data->m2[i] = data->m1[i];
    }

    return(NULL);
}

int matrixSaxpy(int* m1, int* m2, int* a, int* b, int* n, int* nt) {
    clock_t startTime = clock();
	int n2 = (*n)*(*n);
    int perThread = ceil( (double)(n2) / (double)(*nt) );
    int start=0, end=perThread;
    int nthread = 0;
    pthread_t tid[*nt];
    threadData2 targ[*nt];

    // Initiate data for each thread and create
    for (int i=0; i < (*nt); i++) {
        targ[i].m1 = m1;
        targ[i].m2 = m2;
        targ[i].a = (*a);
        targ[i].b = (*b);
        targ[i].istart = start;
        targ[i].iend = end;
        targ[i].n2 = n2;
        pthread_create(&tid[i], NULL, tfunMatrixSaxpy, (void*)&targ[i]);

        start += perThread;
        end += perThread;
        // If more threads than indices, cause loop to break
        if (start >= n2) {
            nthread = i+1;
	    break;
        }
    }

    for (int i=0; i < nthread; i++) {
        pthread_join(tid[i], NULL);
    }

	clock_t endTime = clock();
	int timeTook = (double)(endTime - startTime) / CLOCKS_PER_SEC * pow(10, 6);
	return timeTook;
}

void* tfunMatrixSaxpy(void* arg) {
    threadData2 *data = (threadData2*)arg;
    int j = data->iend;
    int a = data->a;
    int b = data->b;

    // if end exceeds # index, assign end to # index
    if (j > data->n2) {
        j = data->n2;
    }
    
    // Calculate each m1 index and assign to same index in m2
    for (int i = data->istart; i < j; i++) {
        data->m2[i] = (a * data->m1[i]) + b;
    }

    return(NULL);
}

int innerProduct(int* l1, int* v1, int* v2, int* n, int* nt) {
    clock_t startTime = clock();
	int perThread = ceil( (double)(*n) / (double)(*nt) );
    int start=0, end=perThread;
    int nthread = 0;
    innerProductSum = 0;
    pthread_t tid[*nt];
    threadData3 targ[*nt];
    sem_init(&sem, 0, 1);   //init semaphore

    // Initiate data for each thread and create
    for (int i=0; i < (*nt); i++) {
        targ[i].v1 = v1;
        targ[i].v2 = v2;
        targ[i].istart = start;
        targ[i].iend = end;
        targ[i].n = *n;
        
        pthread_create(&tid[i], NULL, tfunInnerProduct, (void*)&targ[i]);
        start += perThread;
        end += perThread;

        // If more threads than indices, cause loop to break
        if (start >= (*n)) {
            nthread = i+1;
            break;
        }
    }

    for (int i=0; i < nthread; i++) {
        pthread_join(tid[i], NULL);
    }
    *l1 = innerProductSum;

	clock_t endTime = clock();
	double timeTook = (double)(endTime - startTime) / CLOCKS_PER_SEC * pow(10, 6);
	return timeTook;
}

void* tfunInnerProduct(void* arg) {
    threadData3 *data = (threadData3*)arg;
    int j = data->iend;

     // if end exceeds # index, assign end to # index
    if (j > data->n) {
        j = data->n;
    }
    
    // Parse indices of v1 & v2, calc product and add to global const
    for (int i = data->istart; i < j; i++) {
        int val1 = data->v1[i];
        int val2 = data->v2[i];
        sem_wait(&sem);
        innerProductSum += (val1 * val2);
        sem_post(&sem);
    }

    return(NULL);
}

void generateMatrix(int *matrix, int n) {
    int n2 = n*n;
    for (int i=0; i<n2; i++) {
        matrix[i] = rand() % 10;    //gen random int 0-9
    }
}

void printMatrix(int *matrix, int n) {
    for (int i=0; i < n*n; i++) {
        if (i % n == 0 && i != 0) {
            printf("\n");
        } else if (i != 0) {
            printf(" ");
        }
        printf("%03d", matrix[i]);
    }
    printf("\n");
}
