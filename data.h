#include<iostream>
#include<string>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <cmath>
#include "svm.h"
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
	struct svm_problem prob_svm;


	Data();
	void setMentionLabels(const double *kValues,const double *cpeMentions,double *yLabels,int relationNumber) const;
	double  getWeightsOfCostSensitiveClassifier(int relationNumber) const;
	/*Non Functionality Functions */
	void myPrint(bool printAll) const;
	void myPrint(double *array,int size) const;
};
int probabilityChooser(double key,double *list,int start,int end,int count,int max);