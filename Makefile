HEADERS = my_header.h bmp_header.h

default: bmp

bmp.o: bmp.c $(HEADERS)
	gcc -c bmp.c -o bmp.o

bmp: bmp.o
	gcc bmp.o -o bmp

clean:
	rm bmp.o
	rm bmp