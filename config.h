#include<iostream>
#include<string>
#include<stdlib.h>
#include "svm.h"

#include "linear.h"
using namespace std;

class Config
{

public:
	double *k ;
	int numberOfEpochs;
	bool findBestC;
	bool costSensitiveClassifier;
	bool findBestCosts;
	/* Find Best C Parameters */
	double C_Min;
	double C_Max;
	int nr_folds;

	/* Cost Sensitive Classifier parameters */
	double minCost;
	double maxCost;
	double stepSize;

	
	struct parameter param;
	struct svm_parameter param_svm;
	string trainingFileName;
	string testingFileName;
	string outputFileDirectory;
	bool isSVM;
	
	Config();
	Config(double k[],int numberOfEpochs,string trainingDataset,string testingDataset);

};