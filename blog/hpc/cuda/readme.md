* Haz una copia de vectorAdd y llámala matrixAdd. Modifica el código para que
implemente la suma de 2 matrices de 5000 x 5000.

* Haz una copia de prodMatVec y llámala prodMatMat. Modifica el código para que
implemente la multiplicación de 2 matrices de 5000 x 5000. En los ejemplos de CUDA
ya existe una versión de este código. Puedes utilizarlo como ejemplo.

a) con un solo eje

cd exercises2/ && ./e 40 && make run
Time spent: 1033767


real	0m13.017s
user	0m1.376s
sys	0m2.148s

b) con dos ejes


using grids!!


Time spent: 9139

http://people.ds.cam.ac.uk/pmb39/GPULectures/Lecture_2.pdf

cd exercises2b/ && ./e 40 && make run


eal	0m13.094s
user	0m1.288s
sys	0m2.127s

[Matrix addition of 1600 elements]
Copy input data from the host memory to the CUDA device
CUDA kernel launch with 3 blocks of (16x16) threads
Total size: 1600 (N=40) Total threads: 2304 (T=16,B=3)Copy output data from the CUDA device to the host memory

http://0-proquestcombo.safaribooksonline.com.jabega.uma.es/book/software-engineering-and-development/9780124159921/chapter-4dot-data-parallel-execution-model/s0015tit_chp004_html#X2ludGVybmFsX0h0bWxWaWV3P3htbGlkPTk3ODAxMjQxNTk5MjElMkZzMDAyMHRpdF9jaHAwMDRfaHRtbCZxdWVyeT0=




