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

	double * sortArray(const double *cpeEntityPairs,int SIZE);
	double *  getMaxCpePerEntityPair(const Data *data);
	double  findBestMacroThreshold(const double *cpeEntityPairs,const Data *data);
	double * getKForEntityPairs(const Data *data,double Threshold);
	double getFscore(const double *predictedEntityLabels,const double *entityLabels,const Data *data);
	double getFScore(Data *data,double threshold);
	double * findLabelsBasedOnEntity(const double *cpeEntityPairs,double bestThreshold,const Data *data);
	
	//predictedEntityLabels <-- findLabels(cpeMentionsTestData,bestThreshold);
	
	//F_score <-- getFscore(predictedEntityLabels,entityLabels);


	/* Experimental functions */
	double findBestMacroThreshold_BasedOnMention(double *cpeEntityPairs,Data *data);
	double * findLabelsBasedOnMentions(const double *cpeMentionsTestData,double bestThreshold,const Data *data);
	int * findLabelsAndKValues(const double *cpeMentionsTestData,double bestThreshold,const Data *data,int *k);
	


};
