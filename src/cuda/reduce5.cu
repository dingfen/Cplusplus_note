#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdlib.h>
#include <stdio.h>

#define NUM_SIZE 1024

template <unsigned int blockSize>
__global__ void reduce5(int *g_idata, int *g_odata) {
	__shared__ volatile int sdata[512];

    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
	
	sdata[i] = g_idata[i] + g_idata[i + blockSize];
	__syncthreads();

	if (blockSize >= 512) {
		if (tid < 256) {sdata[tid] += sdata[tid + 256];} __syncthreads();
	}
	if (blockSize >= 256) {
		if (tid < 128) {sdata[tid] += sdata[tid + 128];} __syncthreads();
	}
	if (blockSize >= 128) {
		if (tid < 64) {sdata[tid] += sdata[tid + 64];} __syncthreads();
	}
	if (tid < 32) {
		if (blockSize >= 64) sdata[tid] += sdata[tid + 32];
		if (blockSize >= 32) sdata[tid] += sdata[tid + 16];
		if (blockSize >= 16) sdata[tid] += sdata[tid + 8];
		if (blockSize >= 8) sdata[tid] += sdata[tid + 4];
		if (blockSize >= 4) sdata[tid] += sdata[tid + 2];
		if (blockSize >= 2) sdata[tid] += sdata[tid + 1];
	}
	if (i == 0) {
		g_odata[0] = sdata[0];
	}
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

    reduce5<512><<<1, 512, NUM_SIZE>>> (a_dev, b_dev);

    cudaMemcpy(b, b_dev, sizeof(int)*NUM_SIZE, cudaMemcpyDeviceToHost);

    cudaDeviceSynchronize();
	for(int i = 0; i < 32; i++)
	     printf("%d\n", b[i]);
    
    cudaFree(a_dev);
    cudaFree(b_dev);

    free(a);
    free(b);
}
