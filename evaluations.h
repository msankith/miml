#include<iostream>
#include"global.h"
#include"stdlib.h"



class Evaluation {
	
	// double[] cpeMentions; //SizeofTraining or testing data
	// double[] cpeEntityPairs;
	// double[] k;
	// int[] predictedEntityLabels;

	public:

	Evaluation();

	double *  getMaxCpePerEntityPair(Data *data);
	
	double *  getMaxCpePerEntityPair(double *cpeMentions,int *mentionsEntityPairCount);
	//cpeEntityPairs <--  getMaxCpePerEntityPair(cpeMentions,entityPairsCount)
	
	double  findBestMacroThreshold(double *cpeEntityPairs,Data *data);
	//bestThreshold <--  findBestMacroThreshold(cpeEntityPairs);
	double findBestMacroThreshold_BasedOnMention(double *cpeEntityPairs,Data *data);

	
	double * getKForEntityPairs(double *cpeMentions,double currentThreshold,Data *data);
	//K <--  getKForEntityPairs(cpeMentions,currentThreshold);
	double * getKForEntityPairs(Data *data,double Threshold);
	
	int * findLabelsBasedOnMentions(double *cpeMentionsTestData,double bestThreshold,Data *data);
	
	
	int * findLabelsBasedOnEntity(double *cpeEntityPairs,double bestThreshold,Data *data);
	
	int * findLabelsAndKValues(double *cpeMentionsTestData,double bestThreshold,Data *data,int *k);
	//predictedEntityLabels <-- findLabels(cpeMentionsTestData,bestThreshold);
	
	double getFscore(int *predictedEntityLabels, int *entityLabels,Data *data);
	//F_score <-- getFscore(predictedEntityLabels,entityLabels);

	double * sortArray(double *cpeEntityPairs,int SIZE);
};
