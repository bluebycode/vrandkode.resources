# https://mop.cv.uma.es/mod/resource/view.php?id=424829

Flynn taxonomy
https://en.wikipedia.org/wiki/Flynn%27s_taxonomy

![](https://raw.githubusercontent.com/vrandkode/vrandkode.resources/master/blog/hpc/flynn_taxonomy.png)

SIMD. A computer which executes the same instructions on multiple pieces of data simultaneously. These are useful with algorithms which can be naturally parallelized, such as vector or matrix computations, image processing and the finite element method. Examples of devices with SIMD architecture are an array processor and a graphics processing unit (GPU).

Each processing unit (PU) is shown for a uni-core or multi-core compute:
![simd](https://upload.wikimedia.org/wikipedia/commons/thumb/2/21/SIMD.svg/500px-SIMD.svg.png)


Steps:

# SIMD support of the processor core

## Vectorization
Editando el `Makefile`

### Auto-vectorization
### Manual vectorization
Añadir vectorizacion de instrucciones de forma manual con lo de AVX 
    
![](https://raw.githubusercontent.com/vrandkode/vrandkode.resources/master/blog/hpc/vmx.png)


### Whole makefile
```
CC = gcc
CFLAGS = -O2
SIMDFLAGS = -ftree-vectorize
SIMDFLAGSVSUCCESS = $(SIMDFLAGS) -fopt-info-vec-optimized
SIMDFLAGSVFAILED = $(SIMDFLAGS) -fopt-info-vec-missed
SIMDFLAGS2 = -msse4
LIBS = -lm 

# Automatic/manual vectorization

#  does not enable the automatic vectorization
noauto:
	$(CC) $(CFLAGS) $(app).c -o noauto$@ $(LIBS)

# does enable the automatic vectorization
auto:
	$(CC) $(CFLAGS) $(SIMDFLAGS) $(app).c -o auto$@v $(LIBS)

# showing information messages about what code sections were successfully vectorized
sucess:
	$(CC) $(CFLAGS) $(SIMDFLAGSVSUCCESS) $(app).c -o sucess_verbose$@v $(LIBS)

# showing information messages about the problems found that precluded vectorization
failed:
	$(CC) $(CFLAGS) $(SIMDFLAGSVFAILED) $(app).c -o failed_verbose$@v $(LIBS)

# vectorization enabled and generating assembler code 
withassembler:
	$(CC) -S $(CFLAGS) $(SIMDFLAGSAUTO) $(app).c $(LIBS)
    
# manual. using the compiler intrinsics to manually program the vector code
manual:
	$(CC) $(CFLAGS) $(SIMDFLAGS2) $(app)-v.c -o manual$@-v $(LIBS)

clean:
	rm -fr *mm *mmv *mm-v *sinx *sinxv *sinx-v dist distv dist-v *.o

```

```
make clean all
make <target> app=<app>
make success mm
```

# Exercices
## Product of two matrices

1. Only compiler optimization
```
gcc -O2 mm.c -o mm
./mm
```

2. Auto-vectorization option
```
gcc -O2 -ftree-vectorize mm.c -o mmv
./mmv
#Besides, the compiler options "-fopt..." and "-S" can be used to obtain information about the vectorization process.
gcc -O2 -S mm.c
gcc -O2 -ftree-vectorize -fopt-info-vec-optimized mm.c -o mmvoptimized
gcc -O2 -ftree-vectorize -fopt-info-vec-missed mm.c -o mmvmissed
```

3. SIMD compiler intrinsics are used to explicitly exploit SIMD parallelis
```
gcc -O2 -msse4 mm-v.c -o mm-v
./mm-v
```

Results are compared and discussed


## Sin() for a large set of numbers using a polynomic expansion.

1. Only compiler optimization
```
gcc -O2 sinx.c -o mm
./mm
```

2. Auto-vectorization option
```
gcc -O2 -ftree-vectorize sinx.c -o sinxv
./sinxv
#Besides, the compiler options "-fopt..." and "-S" can be used to obtain information about the vectorization process.
gcc -O2 -S sinx.c
gcc -O2 -ftree-vectorize -fopt-info-vec-optimized sinx.c -o sinxvoptimized
gcc -O2 -ftree-vectorize -fopt-info-vec-missed sinx.c -o sinxvmissed
```

3. SIMD compiler intrinsics are used to explicitly exploit SIMD parallelis
```
gcc -O2 -msse4 sinx-v.c -o sinx-v
./sinx-v
```

Results are compared and discussed

## Euclidean distance: an array of computations of euclidean distances and the calculation of the maximum value (dist)

1. Only compiler optimization
```
gcc -O2 dist.c -o dist
./dist
```

2. Auto-vectorization option
```
gcc -O2 -ftree-vectorize dist.c -o distv
./distv
#Besides, the compiler options "-fopt..." and "-S" can be used to obtain information about the vectorization process.
gcc -O2 -S dist.c
gcc -O2 -ftree-vectorize -fopt-info-vec-optimized dist.c -o distvoptimized
gcc -O2 -ftree-vectorize -fopt-info-vec-missed dist.c -o distvmissed
```

3. SIMD compiler intrinsics are used to explicitly exploit SIMD parallelis
```
gcc -O2 -msse4 dist-v.c -o dist-v
./dist-v
```
4. Anwers
*Results are compared and discussed*
...

*The report must explain what happens with the auto-vectorization.*
...

*The report must explain how the manual vectorization is accomplished.*
...


