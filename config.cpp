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
	outputFileDirectory="/home/ankith/learning/mtp/code/miml_c/temp/";



	/* Find Best C Settings */
	findBestC=false;
	C_Min=0.5;
	C_Max=32;
	nr_folds=5;
	param.C = 8; /* if findBestC is false , this param.C value will be taken*/
	

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
	else
		param.nr_weight=0; 
	
	param.weight_label[0]=0;
	param.weight_label[1]=1;
	
	/* End Cost sensitive Classifier Settings */

	/*End of Libliner Parameter settings */


	/* Lib SVM Settings */

	isSVM=true;

	param_svm.svm_type = C_SVC;
	param_svm.kernel_type = RBF;
	param_svm.degree = 3;
	param_svm.gamma = 0;	// 1/num_features
	param_svm.coef0 = 0;
	param_svm.nu = 0.5;
	param_svm.cache_size = 1000;
	param_svm.C = 4;
	param_svm.eps = 1e-3;
	param_svm.p = 0.1;
	param_svm.shrinking = 1;
	param_svm.probability = 1;
	param_svm.nr_weight = 0;
	param_svm.weight_label = NULL;
	param_svm.weight = NULL;


	/*ENd  LibSVM settings */ 


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