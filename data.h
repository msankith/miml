#include<iostream>
#include<string>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <cmath>
#include<string.h>
using namespace std;
#define _DATA_HEADER
#include "linear.h"

class Data {

public:
	
	int mentionsCount;
	int entityCount;
	int *mentionsPerEntityPairCount;

	int numberOfRelations;
	double **allLabels; //n*m array , consists of all relation values;
	struct problem prob;


	Data();
	void setMentionLabels(const double *kValues,const double *cpeMentions,double *yLabels) const;
	double  getWeightsOfCostSensitiveClassifier(int relationNumber) const;
	/*Non Functionality Functions */
	void myPrint(bool printAll) const;
	void myPrint(double *array,int size) const;
};
int probabilityChooser(double key,double *list,int start,int end,int count,int max);