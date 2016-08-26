#include"config.h"
#include <math.h>
#define INF HUGE_VAL

using namespace std;


Config::Config()
{
	cout<<"Setting Configuration"<<endl;
	double initialKValues[]={1};
	numberOfEpochs = 1;
	trainingFileName = "someFileName";
	testingFileName =  "";
	outputFileDirectory="/home/ankith/temp/";


	/*Liblinear Parameter Settings */

	param.solver_type = L2R_LR;
	param.C = 2;
	param.eps = 0.000001; 
	param.p = 1;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	param.init_sol = NULL;


	/*End of Libliner Parameter settings */

	int kSize=sizeof(initialKValues)/sizeof(double);
	k=(double *)malloc((sizeof(double)*(kSize+1)));
	for(int itr=0;itr<kSize;itr++)
		k[itr]=initialKValues[itr];
	k[kSize]=0;
	cout<<"Configuration Set"<<endl;
}



Config::Config(double *k,int numberOfEpochs,string trainingDataset,string testingDataset)
{
	this->k=k;
	this->numberOfEpochs=numberOfEpochs;
	this->trainingFileName=trainingDataset;
	this->testingFileName=testingDataset;
}