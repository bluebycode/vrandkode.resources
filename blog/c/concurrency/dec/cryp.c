#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cryp.h>

const unsigned ENC_DELTA = 0x9e3779b9;
const unsigned ENC_INIT  = 0xC6EF3720;

#define DEBUG 0

void decrypt(unsigned int* v, unsigned int* k)
{
    int idx = 0;
    unsigned sum = ENC_INIT;
    while(idx++<32){
        
        v[1] -= ((v[0]<<4) + k[2]) ^ (v[0] + sum) ^ ((v[0]>>5) + k[3]);
		v[0] -= ((v[1]<<4) + k[0]) ^ (v[1] + sum) ^ ((v[1]>>5) + k[1]);
		sum  -= ENC_DELTA;
    }
}

unsigned int save(const char* filename, unsigned int * source, int nbytes)
{
    FILE *fOutput;

    // Abrimos fichero binario
    if((fOutput = fopen(filename,"wb")) == NULL){
		printf("[Error] Abriendo fichero de salida %s\n", filename);
	}

    // Escribimos tantos bloques de 4 bytes indicados pasados por argumento
    unsigned int chunks  = nbytes / sizeof(unsigned int);
    unsigned int writes;
    #if DEBUG
    printf("Escribiendo %d bytes/%d (%d x 4bytes)\n", chunks * sizeof(unsigned int),nbytes, chunks);
    #endif
    if ((writes = fwrite(source, sizeof(unsigned int), chunks, fOutput)) != chunks) {
        printf("[Error] Escribiendo el bloque. El nº escrituras fueron: %d\n", writes);
        fclose(fOutput);
        return 0;
    }

    // Agregamos los bytes restantes
    if (nbytes % sizeof(unsigned int) > 0){
        #if DEBUG
        printf("Escribiendo %d bytes restantes\n", nbytes % sizeof(unsigned int));
        #endif
        if ((writes = fwrite(source, nbytes % sizeof(unsigned int), 1 , fOutput)) != 1) {
            printf("[Error] Escribiendo el bloque. El nº escrituras fueron: %d\n", writes);
            fclose(fOutput);
            return 0;
        }
    }

    fclose(fOutput);
    return nbytes;
}

unsigned int read(const char* filename, unsigned int ** destination)
{
    FILE *fInput;

    if ((fInput = fopen(filename,"rb")) == NULL){
		printf("[Error] Abriendo fichero de entrada %s\n", filename);
	}

    // Al comienzo del fichero encriptado se encuentra almacenado (unsigned int) 
    // el tamaño del fichero original desencriptado.
    unsigned int size, reads;
    if ((reads = fread((unsigned int*) &size, sizeof(unsigned int), 1 ,fInput))!= 1){
        printf("[Error] Leyendo el tamaño del fichero.\n");
        fclose(fInput);
        return 0;
    }
 
    // Calculamos el numero de bloques de 4 bytes para pedir en la lectura del fichero
    // en acorde del tamaño del fichero obtenido.
    unsigned int chunks = size / sizeof(unsigned int);
    *destination = malloc(size);
    if ((reads = fread(*destination, sizeof(unsigned int), chunks, fInput)) != chunks){
        printf("[Error] Leyendo el fichero. El nº de lecturas fueron de %d * 4 bytes = %ld bytes.\n", reads, reads*sizeof(unsigned int));
        fclose(fInput);
        return 0;
    }
    fclose(fInput);
    return size;
}
