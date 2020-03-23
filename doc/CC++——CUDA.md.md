# C\C++——CUDA.md

## device属性

| Property           | Explanation                                                  |
| ------------------ | ------------------------------------------------------------ |
| name               | ASCII string identifying the device                          |
| Major/minor        | major and minor revision numbers<br>defining the device's compute capability |
| totalGlobalMem     | total amount of global memory available<br> on the device in bytes |
| maxThreadsPerBlock | maximum number of threads per block                          |
| maxThreadsDim[3]   | maximum size of each dimension of a block                    |
| maxGridSize        | maximum size of each dimension of a grid                     |
| clockRate          | clock frequency in kilohertz                                 |
| sharedMemPerBlock  | maximum amount of shared memory<br> available to a thread block in bytes |
| Warp size          | warp size for device                                         |

```C++
cudaGetDeviceCount(*int devNo);
cudaGetDeviceProperties(*cudaDeviceProp Prop, int devNo);

Prop.multiProcessorCount	Prop.clockRate	Prop.major	Prop.totalGlobalMem
```

## 性能测试

首先，我们说明一些在CPU中我们测试程序运行时间的方法：

```C++
clock_t start = clock();
	// work load
clock_t end = clock();

double time = (double)(end - start) / CLOCKS_PER_SEC;
```

那么GPU中怎么测试时间呢？

事实上，用`clock`测试GPU的运行时间也是可行的。

## 错误处理

CUDA提供了错误处理机制，几乎每个CUDA API函数都会返回`cudaError_t`类型的值，用来指示此次调用函数是否成功，只有返回值为`cudaSuccess`时，才是成功的。工业界在开发CUDA程序时，这些错误处理检查是必须的，但是若在主程序逻辑全部都加上：

```C++
cudaError_t error = cudaMalloc(...);
if (error != cudaSuccess) {
    // error handeling
}
```

此时，使用C/C++中强大的宏就非常有必要，这里推荐一个写法：

```C++
#define gpuErrchk(ans) { gpuAssert((ans), __FILE__, __LINE__); 

inline void gpuAssert(cudaError_t code , const char *file, int line, bool abort = true) {
    if (code != cudaSuccess) {
        fprintf(stderr, "GPU assert: %s %s %d\n", cudaGetErrorString(code), file, line);
        if (abort) exit(code);
    }
}
```

## 内存管理

一般来说，下面两个函数`cudaMalloc`和`cudaFree`是最常用的，这两个函数的作用分别是分配GPU全局内存和回收内存空间。

```C++
// Allocate memory on the device.
// devPtr --> Pointer to allocated device memory
// size  --> Requested allocation size in bytes
// return --> devPtr
__host__  __device__  cudaError_t 	cudaMalloc(void** devPtr, size_t size)

// Frees memory on the device.
__host__  __device__  cudaError_t 	cudaFree(void* devPtr)
```



```C++
__host__  cudaError_t cudaMallocArray(cudaArray_t* array, const cudaChannelFormatDesc* desc, size_t width, size_t height=0, unsigned int flags=0)
// Allocate an array on the device.    
__host__  cudaError_t cudaFreeArray(cudaArray_t array)
// Frees an array on the device.
```



要在Host端（即CPU端）分配内存，那么最好调用下面的函数。它们与之前函数区别是，分配出来的内存都存在于Host端，且都是page-locked。device端可以直接访问，因此相比于`malloc`，读写带宽更高。可以对`cudaMemcpy`进行加速，但若分配过多内存，会使系统性能下降。

```C++    
__host__  cudaError_t cudaMallocHost(void** ptr, size_t size)
// Allocates page-locked memory on the host.
__host__  cudaError_t cudaFreeHost(void* ptr)
// Frees page-locked memory.
```

```C++
__host__  cudaError_t cudaMallocManaged(void** devPtr, size_t size, unsigned int flags=cudaMemAttachGlobal)
// Allocates memory that will be automatically managed by the Unified Memory system.
```

要完成Host内存与Device内存之间的数据传输，需要使用`cudaMemcpy`函数。其用法与C语言中`memcpy`非常相似，只是多了一个参数来指示内存传递的方向。

`cudaMemcpyKind kind`必须是

- [cudaMemcpyHostToHost](https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__TYPES.html#group__CUDART__TYPES_1gg18fa99055ee694244a270e4d5101e95bdeec295de8a74ac2a74f98ffb6c5d7c7) 
- [cudaMemcpyHostToDevice](https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__TYPES.html#group__CUDART__TYPES_1gg18fa99055ee694244a270e4d5101e95b1a03d03a676ea8ec51b9b1e193617568)
- [cudaMemcpyDeviceToHost](https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__TYPES.html#group__CUDART__TYPES_1gg18fa99055ee694244a270e4d5101e95b5653197602d3455a530db5a7edb1a253)
- [cudaMemcpyDeviceToDevice](https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__TYPES.html#group__CUDART__TYPES_1gg18fa99055ee694244a270e4d5101e95b783338534304281650c6cb1363f5a00a)
- [cudaMemcpyDefault](https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__TYPES.html#group__CUDART__TYPES_1gg18fa99055ee694244a270e4d5101e95b715aff8fb2b8f4f1bb553fee802db57a) 

```C++
// Copies data between host and device.
// dst --> Destination memory address
// src --> Source memory address
// cout --> Size in bytes to copy
// kind --> Type of transfer
__host__  cudaError_t cudaMemcpy(void* dst, const void* src, size_t count, cudaMemcpyKind kind)

```

## 优化示例

对一维向量中的所有元素求和。有多种写法，下面剖析一下具体编程写法对性能的影响。[代码示例](../src/cuda)

我们首先想到，对于所有元素求和，又有大量的线程资源可以使用，那么其中之一的解决方案是**二叉树累加**。即每相邻两个元素的相加由一个线程完成，不断重复这一过程，就可以得到最终的结果。

### 方案一

```C++
__global__ void reduce1(int *g_idata, int *g_odata) {
    extern __shared__ int sdata[];

    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    sdata[tid] = g_idata[i];
    __syncthreads();
    
    for(unsigned int s = 1; s < blockDim.x; s *= 2) {
        if(tid % (2*s) ==0) {
            // 若tid不能被2幂次整除，就不能参与计算
            // 保证了每次迭代后只有一半的线程开始下一次迭代
            sdata[tid] += sdata[tid+s];
        }
        __syncthreads();
    }

    if(tid == 0) {
		g_odata[blockIdx.x] = sdata[0];
    }
}
```

该方案的缺点在于，每次筛选的线程数，都是含有2幂次因子的