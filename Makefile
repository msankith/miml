LIBS = blas/blas.a

all: 
	# g++  -g3 -Wall -fmessage-length=0  -o miml *.cpp
	#g++  -g3 -Wall -fmessage-length=0 -o miml *.cpp $(LIBS)
	g++ -fopenmp -pthread  -o miml main.cpp data.cpp evaluations.cpp config.cpp linear.cpp tron.cpp svm.cpp $(LIBS)


loadData: loadData.cpp 
	g++  -o loadData loadData.cpp linear.cpp tron.cpp $(LIBS)

clean:
	rm *.o miml learn
