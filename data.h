#include<iostream>
#include<string>

using namespace std;
#define _DATA_HEADER

class Data {

public:
	int **entityAllLabels;	
	double **featureVector;
	int *trueEntityLabels;

	int mentionsCount;
	int entityCount;
	int *mentionsPerEntityPairCount;
	double *cpeMentions;
	
	struct Problem problem;


	Data();
	Data(string filename);
	Data(int mentionsCount,int EntityCount,double *cpeMentions, int *mentionsEntityPairCount,int *trueLabels);
};