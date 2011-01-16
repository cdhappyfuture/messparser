objects = ECMG.o connection.o messparser.o messhandler.o
ECMG: $(objects) SCS
	g++ -o ECMG $(objects)  -lboost_thread
ECMG.o : ECMG.cpp ECMG.h connection.h messparser.h messhandler.h
	g++ -c ECMG.cpp
connection.o : connection.cpp connection.h
	g++ -c connection.cpp
messparser.o : messparser.cpp messparser.h
	g++ -c messparser.cpp
messhandler.o : messhandler.cpp messhandler.h
	g++ -c messhandler.cpp
SCS : SCS.o
	g++ -o SCS SCS.o
SCS.o : SCS.cpp messhandler.h messparser.h
	g++ -c SCS.cpp
clean :
	rm ECMG SCS SCS.o $(objects)
