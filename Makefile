
all: miml
	
miml: config.o main.cpp evaluations.o data.o
	g++ -o miml main.cpp config.o evaluations.o data.o

evaluations.o: evaluations.h evaluations.cpp
	g++ -c evaluations.cpp

config.o: config.h config.cpp
	g++ -c config.cpp

data.o: data.h data.cpp
	g++ -c data.cpp

learn: learn.cpp
	g++ -o learn learn.cpp
	./learn

clean:
	rm *.o miml learn