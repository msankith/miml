#include"config.h"


using namespace std;


Config::Config()
{
	cout<<"Setting Configuration"<<endl;
}



Config::Config(double *k,int numberOfEpochs,string trainingDataset,string testingDataset)
{
	this->k=k;
	this->numberOfEpochs=numberOfEpochs;
	this->trainingDataSet=trainingDataset;
	this->testingDataSet=testingDataset;
}