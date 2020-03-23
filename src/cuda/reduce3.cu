#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdlib.h>
#include <stdio.h>

#define NUM_SIZE 1024

__global__ void reduce3(int *g_idata, int *g_odata) {
    __shared__ int sdata[32][32];

    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;

    sdata[blockIdx.x][tid] = g_idata[i];
    __syncthreads();
    
    for(unsigned int s = blockDim.x / 2; s > 0; s /= 2) {
        if(tid < s) {
            sdata[blockIdx.x][tid] += sdata[blockIdx.x][tid+s];
        }
        __syncthreads();
    }

	g_odata[blockIdx.x] = sdata[blockIdx.x][0];

}


int main() {
    int * a = (int*)malloc(sizeof(int)*NUM_SIZE);
    int * b = (int*)malloc(sizeof(int)*NUM_SIZE);

    for(int i = 0; i < NUM_SIZE; i++) {
        a[i] = i;
    }

    int *a_dev;
    int *b_dev;
    cudaSetDevice(0);

    cudaMalloc(&a_dev, sizeof(int)*NUM_SIZE);
    cudaMalloc(&b_dev, sizeof(int)*NUM_SIZE);
    
    cudaMemcpy(a_dev, a, sizeof(int)*NUM_SIZE, cudaMemcpyHostToDevice);

    reduce3 <<<32, 32, NUM_SIZE>>> (a_dev, b_dev);
	reduce3 <<<1, 32, 32>>> (b_dev, b_dev);

    cudaMemcpy(b, b_dev, sizeof(int)*NUM_SIZE, cudaMemcpyDeviceToHost);

    cudaDeviceSynchronize();
	for(int i = 0; i < 32; i++)
	    printf("%d\n", b[i]);
    
    cudaFree(a_dev);
    cudaFree(b_dev);

    free(a);
    free(b);
}
