/**
 * Copyright 1993-2013 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */

/**
 * Vector addition: C = A + B.
 *
 * This sample is a very basic sample that implements element by element
 * vector addition. It is the same as the sample illustrating Chapter 2
 * of the programming guide with some additions like error checking.
 */

#include <stdio.h>

// For the CUDA runtime routines (prefixed with "cuda_")
#include <cuda_runtime.h>

/**
 * CUDA Kernel Device code
 *
 * Computes the vector addition of A and B into C. The 3 vectors have the same
 * number of elements numElements.
 */

__global__ void
prodMul(const float *A, const float *B, float *C, int size)
{
    
    int j = blockDim.x * blockIdx.x + threadIdx.x;
    int i = blockDim.y * blockIdx.y + threadIdx.y;

    unsigned int start_time = clock();

    float sum = 0.0f;
    if (i < size && j < size)
    {
        for (int k = 0; k < size; k++)
        {
           sum += A[i * size + k] * B[k*size + j];
        }
        C[i * size + j] = sum;
        //printf("(%d,%d):  %f \n", i, j, C[i*size + j]);
    }

    unsigned int stop_time = clock();

    if(i == 0 && j==0)
    {
       printf("Time spent: %d\n", stop_time - start_time);
    }
}

#define NUM_ELEMENTS 40

/**
 * Host main routine
 */
int
main(void)
{
    // Error code to check return values for CUDA calls
    cudaError_t err = cudaSuccess;

    // Print the vector length to be used, and compute its size
    int numElements = NUM_ELEMENTS*NUM_ELEMENTS;
    size_t size = numElements * sizeof(float);
    printf("[Matrix addition of %d elements]\n", numElements);

    // Allocate the host input vector A
    float *h_A = (float *)malloc(size);

    // Allocate the host input vector B
    float *h_B = (float *)malloc(size);

    // Allocate the host output vector C
    float *h_C = (float *)malloc(size);

    // Verify that allocations succeeded
    if (h_A == NULL || h_B == NULL || h_C == NULL)
    {
        fprintf(stderr, "Failed to allocate host vectors!\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the host input vectors
    for (int i = 0; i < numElements; ++i)
    {
        h_A[i] = rand()/(float)RAND_MAX;
        h_B[i] = rand()/(float)RAND_MAX;
    }

    // Allocate the device input matrix A
    float *d_A = NULL;
    err = cudaMalloc((void **)&d_A, size);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector A (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Allocate the device input vector B
    float *d_B = NULL;
    err = cudaMalloc((void **)&d_B, size);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector B (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Allocate the device output vector C
    float *d_C = NULL;
    err = cudaMalloc((void **)&d_C, size);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to allocate device vector C (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    //
    // vectors -> device input

    // Copy the host input vectors A and B in host memory to the device input vectors in
    // device memory
    printf("Copy input data from the host memory to the CUDA device\n");
    err = cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to copy vector A from host to device (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    err = cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to copy vector B from host to device (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    //
    // Launch the CUDA Kernel

    // Launch the Vector Add CUDA Kernel
    int threadsPerBlockAxis = 16;
    int blocksPerGrid =(NUM_ELEMENTS + threadsPerBlockAxis - 1) / threadsPerBlockAxis; //196
    printf("CUDA kernel launch with %d blocks of (%dx%d) threads\n", blocksPerGrid, threadsPerBlockAxis, threadsPerBlockAxis);
    printf("Total size: %d (N=%d) Total threads: %d (T=%d,B=%d)",numElements, NUM_ELEMENTS, threadsPerBlockAxis*threadsPerBlockAxis*blocksPerGrid*blocksPerGrid, threadsPerBlockAxis, blocksPerGrid);
    dim3 dimBlock(threadsPerBlockAxis, threadsPerBlockAxis, 1);
    dim3 dimGrid(blocksPerGrid, blocksPerGrid, 1);
    prodMul<<<dimGrid, dimBlock>>>(d_A, d_B, d_C, NUM_ELEMENTS);
    err = cudaGetLastError();

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to launch vectorAdd kernel (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Copy the device result vector in device memory to the host result vector
    // in host memory.
    printf("Copy output data from the CUDA device to the host memory\n");
    err = cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);


    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to copy vector C from device to host (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    
    float * cpu_C = (float *) malloc (size);
    float sum;
    int N = NUM_ELEMENTS;
    for (int i=0; i<N; i++){
        for (int j=0; j <N; j++){
            sum = 0.f;
            for (int k=0; k<N; k++){
                sum += h_A[i*N + k] * h_B[k*N + j];
            }
            cpu_C[i*N + j] = sum;
            //ºfprintf(stdout, "(%d,%d):%f ? %f", i, j, cpu_C[i*N + j],h_C[i*N + j]);
        }
    }
    unsigned int i = 0;
    for (int col=0; col <N; col++){
        for (int row=0; row <N; row++){
            i = col * N + row;
            if (fabs(cpu_C[i] - h_C[i]) > 1e-2)
            {
                fprintf(stderr, "Result verification failed at element %d (%d,%d): %1.5f, %1.5f!\n", i, row, col, cpu_C[i], h_C[i]);
                exit(EXIT_FAILURE);
            }
        }
    }

    printf("Test PASSED\n");


    // Free device global memory
    err = cudaFree(d_A);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to free device vector A (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    err = cudaFree(d_B);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to free device vector B (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    err = cudaFree(d_C);

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to free device vector C (error code %s)!\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Free host memory
    free(h_A);
    free(h_B);
    free(h_C);

    // Reset the device and exit
    err = cudaDeviceReset();

    if (err != cudaSuccess)
    {
        fprintf(stderr, "Failed to deinitialize the device! error=%s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    printf("Done\n");
    return 0;
}

