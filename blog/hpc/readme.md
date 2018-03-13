# SIMD support of the processor core

# Concepts

* Campus virtual: https://mop.cv.uma.es/mod/resource/view.php?id=424829

* Flynn taxonomy: https://en.wikipedia.org/wiki/Flynn%27s_taxonomy

![](https://raw.githubusercontent.com/vrandkode/vrandkode.resources/master/blog/hpc/flynn_taxonomy.png)

*SIMD*. A computer which executes the same instructions on multiple pieces of data simultaneously. These are useful with algorithms which can be naturally parallelized, such as vector or matrix computations, image processing and the finite element method. Examples of devices with SIMD architecture are an array processor and a graphics processing unit (GPU).

Each processing unit (PU) is shown for a uni-core or multi-core compute:
![simd](https://upload.wikimedia.org/wikipedia/commons/thumb/2/21/SIMD.svg/500px-SIMD.svg.png)


# Vectorization
Editando el `Makefile`

## Auto-vectorization
## Manual vectorization
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
## Pre. How to calculate Speed up of latencies
 In order to carry out a more effective analysis on the mentioned scenarios and the
results outlined above, we calculated the speedup on the latencies between the nonoptimized
and optimized versions; with the help of the formula

![](https://wikimedia.org/api/rest_v1/media/math/render/svg/71d9789d12f7c545c7bc5582b52f148a0d16c8de)

```
$ perf stat -r 50 ./mm
Performance counter stats for './mm' (50 runs):
4419.488297 task-clock:u (msec) # 0.999 CPUs utilized
          0 context-switches:u # 0.000 K/sec
          0 cpu-migrations:u # 0.000 K/sec
        818 page-faults:u # 0.185 K/sec
268,375,697 cycles:u # 1.192 GHz
463,552,765 instructions:u # 0.09 insn per cycle
 40,621,256 branches:u # 9.191 M/sec
    130,188 branch-misses:u # 0.32% of all branches

4.422536334 seconds time elapsed 

```

## A. SIMD support of the processor core

```
$ cat /proc/cpuinfo

processor	: 63
vendor_id	: GenuineIntel
cpu family	: 6
model		: 46
model name	: Intel(R) Xeon(R) CPU           X7550  @ 2.00GHz
stepping	: 6
microcode	: 0xa
cpu MHz		: 1064.000
cache size	: 18432 KB
physical id	: 3
siblings	: 16
core id		: 11
cpu cores	: 8
apicid		: 119
initial apicid	: 119
fpu		: yes
fpu_exception	: yes
cpuid level	: 11
wp		: yes
flags		: fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx rdtscp lm constant_tsc arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc aperfmperf pni dtes64 monitor ds_cpl vmx est tm2 ssse3 cx16 xtpr pdcm dca sse4_1 sse4_2 x2apic popcnt lahf_lm ida epb dtherm tpr_shadow vnmi flexpriority ept vpid
bogomips	: 4000.31
clflush size	: 64
cache_alignment	: 64
address sizes	: 44 bits physical, 48 bits virtual
power management:
```

* 32 cores = 8 x 4 = 32 x 2 thread context = 64 cores

* Support vectorial to MMX,sse4. ```vmx est tm2 ssse3 cx16 xtpr pdcm dca sse4_1 sse4_2```

## B. Product of two matrices (MM)

1. Only compiler optimization
```
$ gcc -O2 mm.c -o mm
$ ./mm
Sum for matrix C: 5045830144.0000
Exec time:   1.53517 sec.
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
0.48407 sec
```

| |mmv |mm-v |
|-|-|-|
| time | 0.50073| 0.50654 |

results:

Locality spatial achieved, unrolled loops to block of size = 4, one per intrinsec vector:

```
 for (i = 0; i < N; i++) 
       for (k = 0; k < N; k++) 
          for (j = 0; j < N; j+=4) 
          {
             __m128 bv = _mm_load_ps(&b[k][j]);
             __m128 av = _mm_set1_ps(a[i][k]);
             __m128 cv = _mm_load_ps(&c[i][j]);
             __m128 tmp = _mm_add_ps(cv,_mm_mul_ps(av,bv));
             _mm_store_ps(&c[i][j],tmp);
             //c[i][j] = c[i][j] + a[i][k]*b[k][j];
	  }
```

* Vectorised code ''sinx-v.c'

```gcc -O2 -msse4 sinx-v.c -o sinx-v
hpc002@yuca:~/SIMD-codes$ ./sinx-v 10000000
Using N=10000000 and terms=12
Sum for result: 8932797.0000
Exec time:   0.94219 sec.
```

```
 for (i = 0; i < N; i++) 
       for (k = 0; k < N; k++) 
          for (j = 0; j < N; j+=4) 
          {
             __m128 bv = _mm_load_ps(&b[k][j]);
             __m128 av = _mm_set1_ps(a[i][k]);
             __m128 cv = _mm_load_ps(&c[i][j]);
             __m128 tmp = _mm_add_ps(cv,_mm_mul_ps(av,bv));
             _mm_store_ps(&c[i][j],tmp);
             //c[i][j] = c[i][j] + a[i][k]*b[k][j];
	  }
```

* Assembler generation

$ gcc -O2 -S -msse4 sinx-v.c
```
	cvtsi2sdq       8(%rsp), %xmm1
        cvtsi2sdq       (%rsp),  %xmm0
```
* Vectorial registers: xmm0,xmm1 (https://en.wikibooks.org/wiki/X86_Assembly/SSE)



The code includes the vector intrisecs ```#include <smmintrin.h>```

Results are compared and discussed


## Sin() for a large set of numbers using a polynomic expansion.

1. Only compiler optimization
```
gcc -O2 sinx.c -o mm
./sinx
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


* Optimization using info-vec flags
*-fopt-info-vec-optimized/missed* Prints information about optimised/missed optimization opportunities from vectorization passes on stderr.

```
gcc -O2 -ftree-vectorize -fopt-info-vec-optimized sinx.c -o sinxOptimize
```

result:
```
Analyzing loop at sinx.c:16
Analyzing loop at sinx.c:23
sinx.c:12: note: vectorized 0 loops in function.
Analyzing loop at sinx.c:67
Analyzing loop at sinx.c:58
Vectorizing loop at sinx.c:58
sinx.c:58: note: === vect_do_peeling_for_alignment ===
sinx.c:58: note: niters for prolog loop: MIN_EXPR <(unsigned int) -(((unsigned long) vect_px.28_79 & 15) >> 2) & 3, niters.25_3>Setting upper bound of nb iterations for prologue loop to 2
sinx.c:58: note: === vect_update_inits_of_dr ===
sinx.c:58: note: === vect_do_peeling_for_loop_bound ===Setting upper bound of nb iterations for epilogue loop to 2

sinx.c:58: note: LOOP VECTORIZED.
sinx.c:35: note: vectorized 1 loops in function.
```

* Analyzing loop at sinx.c:23

```
for (i=0; i<N; i++)
```

* Vectorizing loop at sinc.c:58

Compiler vectorised it, but this loop contains just an assignment <= bad!
```
for (i=0; i<N; i++)
    x[i] = 1.0; <-- assignment
```

```
Vectorizing loop at sinx.c:58

sinx.c:58: note: === vect_do_peeling_for_alignment ===
sinx.c:58: note: niters for prolog loop: MIN_EXPR <(unsigned int) -(((unsigned long) vect_px.28_79 & 15) >> 2) & 3, niters.25_3>Setting upper bound of nb iterations for prologue loop to 2

sinx.c:58: note: === vect_update_inits_of_dr ===
sinx.c:58: note: === vect_do_peeling_for_loop_bound ===Setting upper bound of nb iterations for epilogue loop to 2

sinx.c:58: note: LOOP VECTORIZED.
```


3. SIMD compiler intrinsics are used to explicitly exploit SIMD parallelis
```
gcc -O2 -msse4 sinx-v.c -o sinx-v
./sinx-v
```

Results are compared and discussed

* Appendix. Comparative

- O2 Optimization

```
hpc002@yuca:~/SIMD-codes$ perf stat -r 5 ./mm
Sum for matrix C: 5045830144.0000
Exec time:   1.53673 sec.
Sum for matrix C: 5045830144.0000
Exec time:   1.53718 sec.
Sum for matrix C: 5045830144.0000
Exec time:   1.55082 sec.
Sum for matrix C: 5045830144.0000
Exec time:   1.53737 sec.
Sum for matrix C: 5045830144.0000
Exec time:   1.53517 sec.

 Performance counter stats for './mm' (5 runs):

       1545,410842 task-clock (msec)         #    0,994 CPUs utilized            ( +-  0,21% )
               142 context-switches          #    0,092 K/sec                    ( +-  2,76% )
                 0 cpu-migrations            #    0,000 K/sec
             1.200 page-faults               #    0,777 K/sec                    ( +- 10,42% )
     1.597.723.097 cycles                    #    1,034 GHz                      ( +-  0,03% )
       786.756.929 stalled-cycles-frontend   #   49,24% frontend cycles idle     ( +-  0,25% )
       778.541.114 stalled-cycles-backend    #   48,73% backend  cycles idle     ( +-  1,58% )
     3.600.835.033 instructions              #    2,25  insns per cycle
                                             #    0,22  stalled cycles per insn  ( +-  0,01% )
       514.964.651 branches                  #  333,222 M/sec                    ( +-  0,01% )
           665.950 branch-misses             #    0,13% of all branches          ( +-  0,11% )

       1,554860542 seconds time elapsed                                          ( +-  0,19% )

hpc002@yuca:~/SIMD-codes$ perf stat -r 5 ./mmO3
Sum for matrix C: 5045830144.0000
Exec time:   0.45266 sec.
Sum for matrix C: 5045830144.0000
Exec time:   0.48628 sec.
Sum for matrix C: 5045830144.0000
Exec time:   0.49452 sec.
Sum for matrix C: 5045830144.0000
Exec time:   0.44726 sec.
Sum for matrix C: 5045830144.0000
Exec time:   0.51056 sec.

 Performance counter stats for './mmO3' (5 runs):

        487,087482 task-clock (msec)         #    0,991 CPUs utilized            ( +-  2,42% )
                49 context-switches          #    0,101 K/sec                    ( +-  4,95% )
                 0 cpu-migrations            #    0,000 K/sec                    ( +-100,00% )
             1.200 page-faults               #    0,002 M/sec                    ( +- 10,43% )
       503.694.080 cycles                    #    1,034 GHz                      ( +-  2,41% )
       342.174.761 stalled-cycles-frontend   #   67,93% frontend cycles idle     ( +-  5,04% )
       232.551.384 stalled-cycles-backend    #   46,17% backend  cycles idle     ( +-  3,51% )
       909.150.957 instructions              #    1,80  insns per cycle
                                             #    0,38  stalled cycles per insn  ( +-  0,04% )
       130.253.324 branches                  #  267,413 M/sec                    ( +-  0,04% )
           661.636 branch-misses             #    0,51% of all branches          ( +-  0,11% )

       0,491444846 seconds time elapsed                                          ( +-  2,40% )
```




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

## Extra: Ispc compilation



*ispc* compiles a C-based SPMD programming language to run on the SIMD units of CPUs; it frequently provides a 3x or more speedup on CPUs with 4-wide vector SSE units and 5x-6x on CPUs with 8-wide AVX vector units, without any of the difficulty of writing intrinsics code. Parallelization across multiple cores is also supported by ispc, making it possible to write programs that achieve performance improvement that scales by both number of cores and vector unit size.

https://github.com/ispc/ispc

## How to write ispc code

Reference: https://indico.cern.ch/event/267608/contributions/1604226/attachments/478423/661899/ispc.pdf

Examples: https://ispc.github.io/example.html

Matrix multiplication: [mm.ispc](https://github.com/vrandkode/vrandkode.resources/blob/master/blog/hpc/simd_sources/mm.ispc)

## How to compile with ispc

```
make simple
```

Source: https://github.com/ispc/ispc/blob/master/examples/simple/Makefile

Makefile
```
CXX=clang++ -m64
CXXFLAGS=-Iobjs/ -O3 -Wall
ISPC=ispc
ISPCFLAGS=-O2 --arch=x86-64 --target=sse2

default: simple

.PHONY: dirs clean
.PRECIOUS: objs/simple.h

dirs:
	/bin/mkdir -p objs/

clean:
	/bin/rm -rf objs *~ simple

simple: dirs  objs/simple.o objs/simple_ispc.o
	$(CXX) $(CXXFLAGS) -o $@ objs/simple.o objs/simple_ispc.o

objs/simple.o: simple.cpp objs/simple_ispc.h 
	$(CXX) $(CXXFLAGS) -c -o $@ $<

objs/%_ispc.h objs/%_ispc.o: %.ispc
	$(ISPC) $(ISPCFLAGS) $< -o objs/$*_ispc.o -h objs/$*_ispc.h
```

