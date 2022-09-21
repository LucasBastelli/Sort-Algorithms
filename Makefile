PROGRAMS = trab1 trab1-DB

CC = gcc

all:	$(PROGRAMS)

trab1:	trab1.c
	gcc -o trab1 trab1.c

trab1-DB: trab1.c
	gcc -o trab1-DB trab1.c -DDEBUG



clean:
	rm -f $(PROGRAMS) *.o
	rm -f $(PROGRAMS)
