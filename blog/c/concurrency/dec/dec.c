#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cryp.h>

int main(int argc, char**argv){
    printf("\r\x1b[32;1m[+]\x1b[0m @vrandkode \n\n       \x1b[33;1mdecrypt\x1b[0m desencripta ficheros mediante operaciones a nivel de bits.\n\n");
    if (argc < 3){
        perror("No hay suficientes argumentos\n. Intenta usarlo como: ./dec <ficheroentrada> <ficherosalida>\n\n");
        exit(1);
    }

    char* input = argv[1];
    char* output = argv[2];
    printf("\r\x1b[32;1m[+]\x1b[0m Abriendo fichero \x1b[33;1m%s\x1b[0m\n", input);

    unsigned int size;
    unsigned int* buffer;
    if ((size = read(input, &buffer)) == 0)
        exit(1);
    
    // Division del tamaño en bytes por el tamaño de cada unidad de bloque
    // utilizado en el almacenamiento, en este caso de 4 bytes.
    unsigned int chunks = size / sizeof(unsigned int);

    // clave para desencriptar
    unsigned int key[4] = { 128, 129, 130, 131};

    // Comenzamos a realizar iteraciones en bloques de 64 bits.
    unsigned int offset = 0x00;
    unsigned int* block;
    do {
        // Reservamos el bloque de 64 bits 
        block = (unsigned int*) malloc(BLOCK_SIZE);
    
        // Copiamos 64 bits de memoria desde el offset
        memcpy((unsigned int*)block, &buffer[offset], BLOCK_SIZE);

        // Mostramos evolucion
        if (offset*4 % 1024 == 0)
            printf("\r\x1b[32;1m[+]\x1b[0m aplicando bloque %d/%d(\x1b[33;1m%.0f%%\x1b[0m) ...\n", offset*4, size,(double) offset*400/ (double) size);

        // Desencriptamos el bloque
        decrypt(block, key);

        // Copiamos 64 bits desde el bloque desencriptado 
        // a la memoria de nuevo, teniendo en cuenta la posibilidad 
        // de que haya podido añadirse un padding de k bytes.
        memcpy(&buffer[offset], block, (offset + 2 >= chunks) ? BLOCK_SIZE - (size % sizeof(unsigned int)) : BLOCK_SIZE);

        offset +=2; // saltamos un bloque (2 direcciones de 32bits=4bytes)
    } while(offset < chunks);

    unsigned int ret = save(output, buffer, size);
    free(buffer);
    printf("\r\x1b[32;1m[+]\x1b[0m Guardado como \x1b[33;1m%s\x1b[0m.\n",output);
    printf("\r\x1b[32;1m[+]\x1b[0m Hecho.\n");
    exit(ret);
}
