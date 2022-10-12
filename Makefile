PROGRAMS = trab-insertion trab-DB trab-selection trab-merge trab-quick

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

trab-DB: trab.c
	gcc -O0 -o trab-DB trab.c -pthread -DDEBUG -DQUICKSORT



clean:
	rm -f $(PROGRAMS) *.o
	rm -f $(PROGRAMS)
