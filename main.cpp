#include<iostream>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<chrono>

#define sizeT 2000
#define sizeC 5
#define thread_count 4
using namespace std;

void getMatrix(int m [][sizeT]){
    for(int i=0; i<sizeT; i++)
        for(int j=0; j<sizeT; j++)
            m[i][j] = i+j+1;
}

void getConvolutionMatrix(int m [][sizeC]){
    for(int i=0; i<sizeC; i++)
        for(int j=0; j<sizeC; j++)
            m[i][j] = 1;
}

double convolution(int matrix[][sizeT], int matrixC[][sizeC], int matrixResult [][sizeT]){
        int pivot [] = {-4,-3,-2,-1,0,1,2,3,4};
        int temp_i = 0;
        int temp_j = 0;
        int count = 0;
        int total = 0;
        int i=0,j=0,l=0,k=0;
        double omp_start = omp_get_wtime();
#       pragma omp parallel for num_threads(thread_count) private(count,total,temp_i,temp_j,i,j,l,k)
        for(i=0; i<sizeT; i++){
            for(j=0; j<sizeT; j++){
                count = 0;
                total = 0;
                for(k=0; k<sizeC; k++){
                    temp_i = i + pivot[k];
                    if(temp_i >= 0 && temp_i < sizeT){
                        for(l=0; l<sizeC; l++){
                            temp_j = j + pivot[l];
                            if( temp_j >= 0 && temp_j < sizeT){
                                count++;
                                total += (matrix[temp_i][temp_j] * matrixC[k][l]);
                            }
                        }
                    }
                }
                matrixResult[i][j] = total / count;
            }
        }
        double omp_end = omp_get_wtime();
        return omp_end - omp_start;
}

static int matrix [sizeT][sizeT];
static int matrixC [sizeC][sizeC];
static int matrixResult [sizeT][sizeT];

int main (int argc, char *argv[])
{
        getMatrix(matrix);

        getConvolutionMatrix(matrixC);
        double min = 1000000000;
        for (int i = 0; i<30; i++){
                double ans = convolution(matrix, matrixC, matrixResult);
                min = (min > ans)?ans:min;
        }
        cout << min << endl;
        return 0;
}
