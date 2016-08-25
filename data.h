#include<iostream>
#include<string>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <cmath>
#include<string.h>
using namespace std;
#define _DATA_HEADER

#ifndef _LIBLINEAR_STRUCTURES
#define _LIBLINEAR_STRUCTURES
struct feature_node
{
	int index;
	double value;
};

struct problem
{
	int l, n;
	double *y;
	struct feature_node **x;
	double bias;            /* < 0 if no bias term */  
};

enum { L2R_LR, L2R_L2LOSS_SVC_DUAL, L2R_L2LOSS_SVC, L2R_L1LOSS_SVC_DUAL, MCSVM_CS, L1R_L2LOSS_SVC, L1R_LR, L2R_LR_DUAL, L2R_L2LOSS_SVR = 11, L2R_L2LOSS_SVR_DUAL, L2R_L1LOSS_SVR_DUAL }; /* solver_type */

struct parameter
{
	int solver_type;

	/* these are for training only */
	double eps;	        /* stopping criteria */
	double C;
	int nr_weight;
	int *weight_label;
	double* weight;
	double p;
	double *init_sol;
};

struct model
{
	struct parameter param;
	int nr_class;		/* number of classes */
	int nr_feature;
	double *w;
	int *label;		/* label of each class */
	double bias;
};

#endif

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

	/*Non Functionality Functions */
	void myPrint(bool printAll) const;
	void myPrint(double *array,int size) const;
};
int probabilityChooser(double key,double *list,int start,int end,int count,int max);