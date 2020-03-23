#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdlib.h>
#include <stdio.h>

__global__ void reduce1(int *g_idata, int *g_odata) {
    extern __shared__ int sdata[];

    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    sdata[tid] = g_idata[i];
    __syncthreads();
    
    for(unsigned int s = 1; s < blockDim.x; s *= 2) {
        if(tid % (2*s) ==0) {
            sdata[tid] += sdata[tid+s];
        }
        __syncthreads();
    }

    if(tid == 0) {
		g_odata[blockIdx.x] = sdata[0];
    }
}

__global__ void test(int *g_idata, int *g_odata) {
    extern __shared__ int sdata[];
    printf("hello world\n");
}

int main() {
    int * a = (int*)malloc(sizeof(int)*32);
    int * b = (int*)malloc(sizeof(int)*32);

    for(int i = 0; i < 32; i++) {
        a[i] = i;
    }

    int *a_dev;
    int *b_dev;
    cudaSetDevice(0);

    cudaMalloc(&a_dev, sizeof(int)*32);
    cudaMalloc(&b_dev, sizeof(int)*32);
    
    cudaMemcpy(a_dev, a, sizeof(int)*32, cudaMemcpyHostToDevice);

    reduce1 <<<1, 32, 32>>> (a_dev, b_dev);

    cudaMemcpy(b, b_dev, sizeof(int)*32, cudaMemcpyDeviceToHost);

    cudaDeviceSynchronize();
    printf("%d\n", b[0]);
    
    cudaFree(a_dev);
    cudaFree(b_dev);

    free(a);
    free(b);
}
