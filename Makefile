objects = ECMG.o  messparser.o messhandler.o
all: ECMG SCS
ECMG: $(objects)
	gcc -o ECMG $(objects)  -lboost_thread
ECMG.o : ECMG.cpp ECMG.h connection.h messparser.h messhandler.h
	gcc -c ECMG.cpp
messparser.o : messparser.c messparser.h
	gcc -c messparser.cpp
messhandler.o : messhandler.c messhandler.h
	gcc -c messhandler.cpp
SCS : SCS.o
	gcc -o SCS SCS.o
SCS.o : SCS.cpp messhandler. messparser.h
	g++ -c SCS.cpp
clean :
	rm ECMG SCS SCS.o $(objects)
