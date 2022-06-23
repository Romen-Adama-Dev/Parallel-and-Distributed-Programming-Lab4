#include<stdio.h>
#include<omp.h>
#include<stdlib.h>
#include<time.h>
#define N 500

int main()
{

        //Test reduction clause
        int square=0, number, i;
        double start, stop;
        printf("Insert number:\n");
        scanf("%d",&number);
        //how to parallelize it with good final result?
        //#pragma omp parallel for reduction
        start = omp_get_wtime();
        #pragma omp parallel for reduction(+:square)
        for(i=0;i<N;i++)
        {
                square+=number*number;
        }
        printf("Square Reduction=%d\n",square);
        stop = omp_get_wtime();
        printf("Finish Execution= %f\n", stop-start);


        //locks (omp_set_lock)
        square = 0;
        start = omp_get_wtime();
        omp_lock_t writelock;
        omp_init_lock(&writelock);
        #pragma omp parallel for
        for (i = 0; i < N; i++){
                omp_set_lock(&writelock);
                square += number*number;
                omp_unset_lock(&writelock);
        }
        printf("Square lock= %d\n", square);
        stop = omp_get_wtime();
        printf("Finish Execution= %f\n",stop-start);
        //I should  create a setence like mutex to finish the work, so I apply omp_destroy
        omp_destroy_lock(&writelock);


        //atomic test
        square = 0;
        start = omp_get_wtime();
        for(i = 0; i < N; i++){
                #pragma omp atomic
                square += number*number;
        }
        printf("Atomic= %d\n", square);
        stop = omp_get_wtime();
        printf("Finish Execution= %f\n", stop-start);


        //Critical directives

        start = omp_get_wtime();
        #pragma omp critical
        for( i = 0; i < N; i++){
                square += number*number;
        }
        printf("Critical Directives= %d\n", square);
        stop = omp_get_wtime();
        printf("Finish Execution= %f\n", stop-start);
}
//Romen Adama Caetano