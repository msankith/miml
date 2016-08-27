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



	/* Find Best C Settings */
	findBestC=false;
	C_Min=0.5;
	C_Max=32;
	nr_folds=5;
	param.C = 16; /* if findBestC is false , this param.C value will be taken*/
	

	/*Liblinear Parameter Settings */
	param.solver_type = L2R_LR;
	
	param.eps = 0.000001; 
	param.p = 1;
	param.nr_weight = 0;
	param.init_sol = NULL;
	param.nr_thread=6;
	

	param.weight_label=(int *)malloc(sizeof(int)*2);
	param.weight=(double *)malloc(sizeof(double)*2);
	
	/* Cost sensitive Classifier Settings */
	costSensitiveClassifier = false;
	findBestCosts=false;
	minCost=0.1;
	maxCost=1;
	stepSize=0.1;
	param.nr_weight=0; 
		/* Default Weights of sensitive classifier */
	if(findBestCosts==false){
		param.weight[0]=0.1; /* Default costs */ 
		param.weight[1]=0.9;
		param.nr_weight=2;	
		stepSize=maxCost;
	}
	if(costSensitiveClassifier)
		param.nr_weight=2;	
	
	param.weight_label[0]=0;
	param.weight_label[1]=1;
	
	/* End Cost sensitive Classifier Settings */

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