#include<iostream>
#include<string>

using namespace std;

class Config
{

	double *k ;
	int numberOfEpochs;
	string trainingDataSet;    //File Path
	string testingDataSet;   //File Path
	double *trainingTolerance;
	
public:
	
	double bestMacroThreshold;
	double bestMacroFScore;
	
	double bestMicroThreshold[];
	double bestMicroFscore[];
	
	
	Config();
	Config(double k[],int numberOfEpochs,string trainingDataset,string testingDataset);

};