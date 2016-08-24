#include<iostream>
#include<string>
#include<stdlib.h>


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

struct model
{
	struct parameter;
	int nr_class;		/* number of classes */
	int nr_feature;
	double *w;
	int *label;		/* label of each class */
	double bias;
};


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

#endif

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