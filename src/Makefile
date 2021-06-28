all: libsgemm.so libfileread.so test init

libfileread.so: fileread.c
	gcc -Wall -c fileread.c -fPIC 
	gcc -shared fileread.o -o libfileread.so
	ar rcs libfileread.a fileread.o

libsgemm.so: sgemm.c sgemm.h
	gcc -Wall -c sgemm.c -fPIC -g
	gcc -shared sgemm.o -o libsgemm.so
	ar rcs libsgemm.a sgemm.o


test: test.c
	gcc -Wall test.c -o test -lfileread -L. -lsgemm -I. -ljansson

init: init.c
	gcc -Wall init.c -o init

clean:
	rm -rf *.o *.so *.a test test_static init
