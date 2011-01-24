CFLAGS=-g
objects = ECMG.o  messparser.o messhandler.o
all: ECMG SCS
ECMG: $(objects)
	gcc -o ECMG $(objects) -lpthread
ECMG.o : ECMG.c ECMG.h messparser.h messhandler.h
	gcc -c ECMG.c
messparser.o : messparser.c messparser.h
	gcc -c messparser.c
messhandler.o : messhandler.c messhandler.h
	gcc -c messhandler.c
SCS : SCS.o $(objects)
	gcc -o SCS SCS.o messparser.o
SCS.o : SCS.c messhandler.h messparser.h
	gcc -c SCS.c messparser.c
clean :
	rm ECMG SCS SCS.o $(objects)
