
Openmp reference: https://mop.cv.uma.es/pluginfile.php/534263/mod_resource/content/1/OpenMP-4.5.pdf

## Introduction

* The OpenMP API uses the fork-join model of parallel execution

* An OpenMP program begins as a single thread of execution, called an initial thread --> executes sequentialy

* the implicit parallel region => runs at _host device_

* each device has own threads <- they can't not migrate to other devices

* **target** region -> target devices

* thread -> parallel -> create zero + additional threads -> becomes master of team

* parallel -> code of each task is inside -> each task assigned to different thread

* can be nested

* proc_bind -> specify places to use the threads in the team

* default barrier, unless nowait found.

* task -> new explicit task will generate

* simd -> the iteraations of the loop -> executed concurrently using SIMD

* cancel -> cancel activated in task belongs to taskgroup and continue next task region

* cancel -> other tasks continue except cancellation point found

* OpenMP specification makes no guarantee that input or output to the same file is synchronous
38 when executed in parallel

## Memory model



## Directives

Use of ```#pragma omp <directive> <clauses>, <clause-n>```

* Case sensitive
* Applied to structured block
* A list item is a variable or array section. An extended list item is a list item or a function name.

Sentinels:

```!$omp | c$omp | *$omp```


example:
in the following example, the three formats for specifying the directive are equivalent (the
12 first line represents the position of the first 9 columns):
```
c23456789
14 !$omp parallel do shared(a,b,c)
15
16 c$omp parallel do
17 c$omp+shared(a,b,c)
```


```
!23456789
6 !$omp parallel do &
7 !$omp shared(a,b,c)
8
9 !$omp parallel &
10 !$omp&do shared(a,b,c)
11
12 !$omp paralleldo shared(a,b,c)
```

* A stand-alone directive may be placed only at a point where a base language executable
21 statement is allowed.a stand-alone directive may not be used in place of the statement following an if,
3 while, do, switch, or label

## Parallel (pag 56)

construct starts parallel execution.

```#pragma omp parallel [clause[ [,] clause] ... ] new-line
structured-block

if([parallel :] scalar-expression)
5 num_threads(integer-expression)
6 default(shared | none)
7 private(list)
8 firstprivate(list)
9 shared(list)
10 copyin(list)
11 reduction(reduction-identifier : list)
12 proc_bind(master | close | spread)
```

* team of threads is created
* number. if and num_threads
* proc_bind. set threads to places within the places listed in the place-partition-var ICV for the implicit task of the encountering thread

* thread numbers uniquely identify each thread, how to get number **omp_get_thread_num*

http://ebookcentral.proquest.com.uma.debiblio.com/lib/bibliotecauma-ebooks/reader.action?docID=3338748&query=

# Exercise

```
gcc -fopenmp -o example example.c
```

```
#include <omp.h>
#include <stdio.h>
int main()
{
   #pragma omp parallel
   {
     int ID=omp_get_thread_num();
     printf("hello(%d)", ID);
     printf("world(%d)\n", ID);
   }
}
```

```
$ ./example
hello(2)world(2)
hello(1)world(1)
hello(0)world(0)
hello(3)world(3)
```


