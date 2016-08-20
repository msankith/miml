#include<iostream>


using namespace std;
#define _DATA_HEADER

class Data {

public:
	
	double **featureVector;
	int *trueEntityLabels;

	int mentionsCount;
	int entityCount;
	int *mentionsPerEntityPairCount;
	double *cpeMentions;
	


	Data();
	Data(int mentionsCount,int EntityCount,double *cpeMentions, int *mentionsEntityPairCount,int *trueLabels);
};