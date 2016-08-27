#include<iostream>
#ifndef _DATA_HEADER
#define _DATA_HEADER
#include"data.h"
#endif


#include"stdlib.h"



class Evaluation {
	
	// double[] cpeMentions; //SizeofTraining or testing data
	// double[] cpeEntityPairs;
	// double[] k;
	// int[] predictedEntityLabels;

	public:

	Evaluation();

	// double * sortArray(const double *cpeEntityPairs,int SIZE);
	double *  getMaxCpePerEntityPair(const Data *data,const double *cpeMentions);
	double  findBestMacroThreshold(const double *cpeEntityPairs,const Data *data,int relationNumber,double *fscore);
	double * sortArray(const double *cpeEntityPairs,int SIZE);
	double * findLabelsBasedOnEntity(const double *cpeEntityPairs,double bestThreshold,int entityCount);
	double * findLabelsBasedOnMentions(const double *cpeMentions,double threshold,const Data *data);
	double getFscore(const double *predictedEntityLabels,const double *entityLabels, int entityCount);
	double * getKForEntityPairs(const Data *data,double Threshold,double *cpeMentions,int relationNumber);
	double getFScore(const Data *data,double threshold,double *cpeMentions,int relationNumber);
	// double  findBestMacroThreshold(const double *cpeEntityPairs,const Data *data,int relationNumber);
	// double * getKForEntityPairs(const Data *data,double Threshold,double *cpeMentions);
	// double getFscore(const double *predictedEntityLabels,const double *entityLabels,const Data *data);
	// double getFScore(Data *data,double threshold,double *cpeMentions,int relationNumber);
	// double * findLabelsBasedOnEntity(const double *cpeEntityPairs,double bestThreshold,const Data *data);
	
	// //predictedEntityLabels <-- findLabels(cpeMentionsTestData,bestThreshold);
	
	// //F_score <-- getFscore(predictedEntityLabels,entityLabels);


	// /* Experimental functions */
	// double findBestMacroThreshold_BasedOnMention(double *cpeEntityPairs,Data *data,int relationNumber);
	// double * findLabelsBasedOnMentions(const double *cpeMentionsTestData,double bestThreshold,const Data *data);
	// int * findLabelsAndKValues(const double *cpeMentionsTestData,double bestThreshold,const Data *data,int *k);
	


};
