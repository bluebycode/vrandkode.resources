#ifndef __CRYP_H__
#define __CRYP_H__

#define BLOCK_SIZE  (unsigned int) sizeof(unsigned int) * 2 // 64 bits

void decrypt(unsigned int* v, unsigned int*k);

unsigned int save(const char* filename, unsigned int * source, int nbytes);

unsigned int read(const char* filename, unsigned int ** destination);

#endif /* __CRYP_H__ */
