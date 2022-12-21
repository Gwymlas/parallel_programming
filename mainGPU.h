#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "cublas_v2.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

void printmat(float* m, int n)
{
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < n; i++)
            printf("%f ", m[i + j * n]);
        printf("\n");
    }
}

int main(int argc, char* argv[])
{
    const int BLOCK_SIZE = 5;
  
    int n = 2000;

    if ((n < BLOCK_SIZE) || (n % BLOCK_SIZE))
    {
        fprintf(stderr, "Invalid n: %d\n", n);
        printf("n must be > %d and a multiplier of %d\n",
            BLOCK_SIZE, BLOCK_SIZE);
        return 1;
    }

    printf("n = %d\n", n);

    int n2 = n * n, n2b = n2 * sizeof(float);

    // Allocate host memory
    float* a = (float*)malloc(n2b);
    float* b = (float*)malloc(n2b);
    float* c1 = (float*)malloc(n2b);
    float* c2 = (float*)malloc(n2b);

    double dinvrandmax = 1.0 / RAND_MAX;
    for (int i = 0; i < n2; i++)
    {
        a[i] = rand() * dinvrandmax;
        b[i] = rand() * dinvrandmax;
    }

    // Allocate device memory
    float* adev = NULL;
    cudaError_t cuerr = cudaMalloc((void**)&adev, n2b);
    if (cuerr != cudaSuccess)
    {
        fprintf(stderr, "Cannot allocate device array for a: %s\n",
            cudaGetErrorString(cuerr));
        return 1;
    }
    float* bdev = NULL;
    cuerr = cudaMalloc((void**)&bdev, n2b);
    if (cuerr != cudaSuccess)
    {
        fprintf(stderr, "Cannot allocate device array for b: %s\n",
            cudaGetErrorString(cuerr));
        return 1;
    }
    float * cdev = NULL;
    cuerr = cudaMalloc((void**)&cdev, n2b);
    if (cuerr != cudaSuccess)
    {
        fprintf(stderr, "Cannot allocate device array for c: %s\n",
            cudaGetErrorString(cuerr));
        return 1;
    }

    // Create cuda event handles
    cudaEvent_t start, stop;
    float gpuTime = 0.0f;
    cuerr = cudaEventCreate(&start);
    if (cuerr != cudaSuccess)
    {
        fprintf(stderr, "Cannot create CUDA start event: %s\n",
            cudaGetErrorString(cuerr));
        return 1;
    }
    cuerr = cudaEventCreate(&stop);
    if (cuerr != cudaSuccess)
    {
        fprintf(stderr, "Cannot create CUDA end event: %s\n",
            cudaGetErrorString(cuerr));
        return 1;
    }

    cuerr = cudaMemcpy(adev, a, n2b, cudaMemcpyHostToDevice);
    if (cuerr != cudaSuccess)
    {
        fprintf(stderr, "Cannot copy a array from host to device: %s\n",
            cudaGetErrorString(cuerr));
        return 1;
    }
    cuerr = cudaMemcpy(bdev, b, n2b, cudaMemcpyHostToDevice);
    if (cuerr != cudaSuccess)
    {
        fprintf(stderr, "Cannot copy b array from host to device: %s\n",
            cudaGetErrorString(cuerr));
        return 1;
    }

    // Measure time of kernel execution
    cuerr = cudaEventRecord(start, 0);
    if (cuerr != cudaSuccess)
    {
        fprintf(stderr, "Cannot record CUDA event: %s\n",
            cudaGetErrorString(cuerr));
        return 1;
    }

    // Set kernel launch configuration
    dim3 threads(BLOCK_SIZE, BLOCK_SIZE);
    dim3 blocks( n / threads.x, n / threads.y);

    kernel<<<blocks, threads>>>(adev, bdev, n, cdev);
    cuerr = cudaGetLastError();
    if (cuerr != cudaSuccess)
    {
        fprintf(stderr, "Cannot launch CUDA kernel: %s\n",
            cudaGetErrorString(cuerr));
        return 1;
    }

    cuerr = cudaDeviceSynchronize();
    if (cuerr != cudaSuccess)
    {
        fprintf(stderr, "Cannot synchronize CUDA kernel: %s\n",
            cudaGetErrorString(cuerr));
        return 1;
    }

    cuerr = cudaEventRecord(stop, 0);
    if (cuerr != cudaSuccess)
    {
        fprintf(stderr, "Cannot copy c array from device to host: %s\n",
            cudaGetErrorString(cuerr));
        return 1;
    }
    cuerr = cudaMemcpy(c1, cdev, n2b, cudaMemcpyDeviceToHost);
    if (cuerr != cudaSuccess)
    {
        fprintf(stderr, "Cannot copy c array from device to host: %s\n",
            cudaGetErrorString(cuerr));
        return 1;
    }

    cuerr = cudaEventElapsedTime(&gpuTime, start, stop);
    printf("time spent: %.2f millseconds\n", gpuTime);
    printf("BLOCK_SIZE = %d\n", BLOCK_SIZE);


    if (n <= 8)
    {
        printf("A =\n"); printmat(a, n);
        printf("B =\n"); printmat(b, n);
        printf("C1 = \n"); printmat(c1, n);
        printf("C2 = \n"); printmat(c2, n);
    }


    // Release resources
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    cudaFree(adev);
    cudaFree(bdev);
    cudaFree(cdev);
    free(a);
    free(b);
    free(c1);
    free(c2);

    return 0;
}
