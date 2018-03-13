CC = gcc
CFLAGS = -O2
SIMDFLAGS = -ftree-vectorize
SIMDFLAGS2 = -msse4
LIBS = -lm


mm: mm.c
	$(CC) $(CFLAGS) mm.c -o $@ $(LIBS)

mmv: mm.c
	$(CC) $(CFLAGS) $(SIMDFLAGS) mm.c -o $@ $(LIBS)

mm-v: mm-v.c
	$(CC) $(CFLAGS) $(SIMDFLAGS2) mm-v.c -o $@ $(LIBS)

sinx: sinx.c
	$(CC) $(CFLAGS) sinx.c -o $@ $(LIBS)

sinxv: sinx.c
	$(CC) $(CFLAGS) $(SIMDFLAGS) sinx.c -o $@ $(LIBS)

sinx-v: sinx-v.c
	$(CC) $(CFLAGS) $(SIMDFLAGS2) sinx-v.c -o $@ $(LIBS)

dist: dist.c
	$(CC) $(CFLAGS) dist.c -o $@ $(LIBS)

distv: dist.c
	$(CC) $(CFLAGS) $(SIMDFLAGS) dist.c -o $@ $(LIBS)

dist-v: dist-v.c
	$(CC) $(CFLAGS) $(SIMDFLAGS2) dist-v.c -o $@ $(LIBS)

clean:
	rm -fr mm mmv mm-v sinx sinxv sinx-v dist distv dist-v *.o
