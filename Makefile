PROGRAMS = trab-insertion trab-DB trab-selection trab-merge trab-quick trab-heap

CC = gcc

all:	$(PROGRAMS)

trab-insertion:	trab.c
	gcc -O3 -o trab-insertion trab.c -pthread -DINSERTIONSORT

trab-selection:	trab.c
	gcc  -O3 -o trab-selection trab.c -pthread -DSELECTIONSORT

trab-merge:	trab.c
	gcc -O3 -o trab-merge trab.c -pthread -DMERGESORT

trab-quick:	trab.c
	gcc -O3 -o trab-quick trab.c -pthread -DQUICKSORT

trab-heap:	trab.c
	gcc -O3 -o trab-heap trab.c -pthread -DHEAPSORT

trab-DB: trab.c
	gcc -O0 -o trab-DB trab.c -pthread -DDEBUG -DMERGESORT



clean:
	rm -f $(PROGRAMS) *.o
	rm -f $(PROGRAMS)
