#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <smmintrin.h>


#define N 600000

double gettime(void)
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec + 1e-6*tv.tv_usec;
}

int main(int argc, char *argv[])
{
    float r[N], a[N], b[N];
    float max = 0.0;
    int i;
    double dtime;

    // Initialize input matrices
    for (i = 0; i < N; i++)
    {
       a[i] = (float) i;
       b[N-i-1] = (float) 2*i;
    }

    // Loop
    dtime = gettime();

    __m128 av, bv;
    __m128 ctev = _mm_set1_ps((float) 0.5); // {0.5, 0.5, 0.5, 0.5}
    __m128 maxi = _mm_set1_ps((float) 0.0); // {0.0, 0.0, 0.0, 0.0}
    for (i = 0; i < N; i+=4)
    {
 	__m128 av = _mm_load_ps(&a[i]);
 	__m128 bv = _mm_load_ps(&b[i]);

	// r[i] = sqrtf(a[i]*a[i] + b[i]*b[i]) + 0.5;
	__m128 rv =_mm_add_ps(_mm_sqrt_ps(_mm_add_ps(_mm_mul_ps(av,av),_mm_mul_ps(bv,bv))), ctev);
	_mm_store_ps(&r[i], rv);

	// if (r[i] > max) max = r[i];
	maxi = _mm_max_ps(rv, maxi);
    }
    _mm_store_ss(&max, maxi);

    dtime = gettime() - dtime;

    // Print results
    printf("r: %f %f %f %f %f %f\n",r[0],r[1],r[2],r[3],r[1000],r[1001]);
    printf("max: %f\n",max);
    printf("Exec time: %9.5f sec.\n",dtime);
}
