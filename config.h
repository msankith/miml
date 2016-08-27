#include<iostream>
#include<string>
#include<stdlib.h>


#include "linear.h"
using namespace std;

class Config
{

public:
	double *k ;
	int numberOfEpochs;
	
	
	struct parameter param;
	string trainingFileName;
	string testingFileName;
	string outputFileDirectory;
	
	
	Config();
	Config(double k[],int numberOfEpochs,string trainingDataset,string testingDataset);

};