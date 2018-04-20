* [Cuda](#cuda)
* [Openmp (canny)](#canny-optimizations)

## Cuda

## 1. Standard multiplication

- Increasing the dimension up to size*size of devices/host arrays
- CPU version added at the bottom of code

## 2. Two approaches

### 2.a 1D Approach [prodMatMat2a.cu](prodMatMat2a.cu)

Standard implementation

- The decice current thread calculates the per row multiplication
- Drawbacks: loop of j->N

### 2.b 2D Approach [prodMatMat2b.cu](prodMatMat2b.cu)

Using blocks of TxT threads

dim3 dimBlock(threadsPerBlockAxis, threadsPerBlockAxis, 1);
    dim3 dimGrid(blocksPerGrid, blocksPerGrid, 1);
    prodMul<<<dimGrid, dimBlock>>>(d_A, d_B, d_C, NUM_ELEMENTS);

* Drawbacks:
  - best only for square matrixs
  - complex design of device memory, precalculation of dimensions of blocks

* Highest good numbers!

## 3. shared

Following the 2a approach but it would be improved using the 2b approach (x.y)

The shared vector contains a block of consecutive cells of B each block dimensions to work share amonts the threads:

 device thread.x ==0    * 0 1 2 3
                          1 4 5 6
                          2 7 8 9
                        * 0 10 11 12
                          1 13 14 15
                          2 16 17 18

- Possible improvements: shared A
- Using 2D approach


## Canny optimizations

[canny2.c](canny2.c)

* Optimizations:

  - Higher in "convolution" function
    * Conditional + non conditional nested loop -> pragma for
    * Shared variables: min, max, out shared between the loops
    * Private variables: (loop indexes) i,j, (temporally ones) pixel, c

  - canny_edge_detection. optimizations
    * Isolated loops on same level of functions:
        - Non-maximum suppression -> parallel for

  - More: using tasks?

  - save_bmps: In the code: replace fwrite but using memcpy + buffer instead --> save memory
