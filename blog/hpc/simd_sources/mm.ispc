
/*
Initialise:
float a[N][N], b[N][N], c[N][N];
    int i,j,k;
    float sum = 0.0;
    double dtime;

    // Initialize input matrices
    for (i = 0; i < N; i++)
       for (j = 0; j < N; j++)
       {
          a[i][j] = b[i][j] = 3.14;
          c[i][j] = 0.0;
       }
*/
export void init(uniform float aout[], uniform float bout[],uniform float cout[]
                   uniform int n) {
    float v = 3.14;
    foreach (i = 0 ... n) {
        foreach (j = 0 ... n) {
            aout[i,j] = v;
            bout[i,j] = v;
            cout[i,j] = 0.0
        }
    }
}

/**
Multiplication:
for (i = 0; i < N; i++) 
       for (k = 0; k < N; k++) 
          for (j = 0; j < N; j++) 
             c[i][j] = c[i][j] + a[i][k]*b[k][j];
*/

export void mult(uniform float ain[], uniform float bin[],uniform float cin[],uniform float cout[], 
                   uniform int n) {
    foreach (i = 0 ... n) {
        foreach (k = 0 ... n) {
          foreach (j = 0 ... n) {
             cout[i,j] = cin[i,j] + ain[i,k]*bin[k,j]
          }
        }
    }
}

/**

    for (i = 0; i < N; i++)
       for (j = 0; j < N; j++)
          sum += c[i][j];
          
*/

export void sum(uniform float cin[], uniform float sum
                   uniform int n) {
    foreach (i = 0 ... n) {
        foreach (j = 0 ... n) {
            sum = sum + cin[i,j];
        }
    }
}

