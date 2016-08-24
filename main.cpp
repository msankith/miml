#include<iostream>
#include"config.h"
#include"evaluations.h"
#include"global.h"
#include "linear.h"

using namespace std;
#define SIZE(A) (sizeof(A)/sizeof(*(A)))



struct feature_node **trainingMentionsFeatures;
int *trainingMentionsPerEntityPair;//trainingMentionsPerEntityPair[#entities];
double **trainingLabelMatrix;//trainingLabelMatrix[#relations][#entities];
//Changing to double
int trainingEntityPairsCount;
int trainingMentionsCount;

struct feature_node **testingMentionsFeatures;
int *testingMentionsPerEntityPair;//testingMentionsPerEntityPair[#entities];
double **testingLabelMatrix;//testingLabelMatrix[#relations][#entities];
int testingEntityPairsCount;
int testingMentionsCount;

int numberOfRelations=1;

void loadData(string fname,struct feature_node **vector,int *mentionsPerEntityPair,double **labelMatrix);
void coreLogic(int relationNumber,Config *config,Data *data,int *mentionsPerEntityPair,double *kValues);
double *getCpe(struct model *relationModel,Data *data);
int main()
{
	Config config = Config();
	loadData(config.trainingFileName,trainingMentionsFeatures,trainingMentionsPerEntityPair,trainingLabelMatrix);
	loadData(config.testingFileName,testingMentionsFeatures,testingMentionsPerEntityPair,testingLabelMatrix);
	
	for(int relationNumber=0;relationNumber<numberOfRelations;relationNumber++)
	{
		double *kValues = (double *)malloc(sizeof(double)*trainingEntityPairsCount);
		Data *data = new Data(trainingMentionsFeatures,trainingLabelMatrix[relationNumber],trainingMentionsPerEntityPair,trainingEntityPairsCount,trainingMentionsCount); 
		// CPE Mentions should be intiailzed to constant value
		coreLogic(relationNumber,&config,data,trainingMentionsPerEntityPair,kValues);
	}
	
}


void coreLogic(int relationNumber,Config *config,Data *data,int *mentionsPerEntityPair,double *kValues)
{
	//Data data = new Data(mentionsFeatureVector,labelMatrix[relationNumber]);
	double bestThreshold;
	struct model *bestRelationModel;
	double bestFScore=0;
	double bestK;
	for(int itrKrange=0;config->k[itrKrange]!=0;itrKrange++)
	{
		double initialK = config->k[itrKrange];
		struct model *relationModel;
		Evaluation eval = Evaluation();		
		double threshold;
		for(int epoch=0;epoch<config->numberOfEpochs;epoch++)
		{
			data->setMentionLabels(kValues,mentionsPerEntityPair);
			relationModel= train(&data->prob,&config->param);
			data->cpeMentions= getCpe(relationModel,data); 
			double *cpeEntityPairs = eval.getMaxCpePerEntityPair(data);
			threshold= eval.findBestMacroThreshold(cpeEntityPairs,data);
			kValues= eval.getKForEntityPairs(data,threshold);
		}
		struct Data *testingData = new Data(testingMentionsFeatures,testingLabelMatrix[relationNumber],testingMentionsPerEntityPair,testingEntityPairsCount,testingMentionsCount) ;
		testingData->cpeMentions = getCpe(relationModel,testingData);
		double fScore= eval.getFScore(testingData,threshold); 
		if(fScore>bestFScore)
		{
			bestFScore=fScore;
			bestRelationModel=relationModel;
			bestThreshold=threshold;
			bestK=initialK;
		}
	}
}


void loadData(string fname,struct feature_node **vector,int *mentionsPerEntityPair,double **labelMatrix){

}


double *getCpe(struct model *relationModel,Data *data)
{
	double *prob_estimates=NULL;
	double *cpeMentions = (double *)malloc(sizeof(double)*data->prob.l);
	prob_estimates = (double *) malloc(relationModel->nr_class*sizeof(double));
	for(int itr=0;itr<data->prob.l;itr++) //prob.l gives number of mentions
	{	
		double predict_label = predict_probability(relationModel,data->prob.x[itr],prob_estimates);
		cpeMentions[itr]=prob_estimates[0];
	}
	return cpeMentions;
}

































// Data *trainingData;

// void print(Data *d)
// {
// 	int i=0;
// 	cout<<"Entity Count   "<<d->entityCount<<endl;
// 	cout<<"Mentions Count   "<<d->mentionsCount<<endl;
// 	cout<<"Mentions per entity  "<<endl;
// 	while(i<d->entityCount)
// 	{
// 		cout<<d->mentionsPerEntityPairCount[i++]<<" ";
// 	}
// 	cout<<endl;

// 	cout<<"TRUE ENTITY LABELS"<<endl;
// 	i=0;
// 	while(i<d->entityCount)
// 	{
// 		cout<<d->trueEntityLabels[i++]<<" ";
// 	}
// 	cout<<endl;


// 	cout<<"-------"<<endl;
// 	i=0;
// 	while(i<d->mentionsCount)
// 		cout<<d->cpeMentions[i++] <<" ";

// 	cout<<endl<<endl<<"CPE MENTIONS"<<endl;
// 	i=0;
// 	int itr=0;
// 	while(i<d->entityCount)
// 	{
// 		int j=0;
// 		while(j<d->mentionsPerEntityPairCount[i])
// 		{
// 			cout<<d->cpeMentions[itr++]<<" ";
// 			j++	;
// 		}
// 		i++;
// 		cout<<"\t";
// 		//cout<<d->mentionsPerEntityPairCount[i++]<<" ";
// 	}
	
// 	cout<<endl;
	
// }
// int main2()
// {
	
// 	srand (time(NULL));

// 	// Data::entityAllLabels;// = (int *)malloc(sizeof(int *)* 1);
// 	Data randomData= Data("dataset/reidel_trainSVM.data_bck");
// 	// print(&randomData);

// 	// Evaluation e = Evaluation();

// 	// double *cpeEntityPairs= e.getMaxCpePerEntityPair(&randomData);
// 	// double threshold= e.findBestMacroThreshold(cpeEntityPairs,&randomData);
// 	// double *k = e.getKForEntityPairs(&randomData,threshold);

// 	// double *sortedEntityPairs = e.sortArray(cpeEntityPairs,randomData.entityCount);
// 	// int i=0;
// 	// while(i<randomData.entityCount)
// 	// 	cout<<sortedEntityPairs[i++]<<" ";
// 	// cout<<endl;
	
// 	// cout<<"k values"<<endl;
// 	// i=0;
// 	// while(i<randomData.entityCount)
// 	// 	cout<<k[i++]<<" ";
// 	// cout<<endl;
	

// 	// cout<<"Best threshold "<< threshold<<"\t"<<endl<<endl<<endl;

// 	// int mentionsEntityPairCount[]={2,3,2};
// 	// int trueLabels[]={1,0,1};
// 	// double cpeMentions[]={1,2,1,8,4,6,7};

// 	// trainingData =new  Data(SIZE(cpeMentions),SIZE(mentionsEntityPairCount),cpeMentions,mentionsEntityPairCount,trueLabels);
// 	// Evaluation e = Evaluation();

// 	// //e.getMaxCpePerEntityPair(cpeMentions,mentionsEntityPairCount);
// 	// double *cpeEntityPairs=e.getMaxCpePerEntityPair(trainingData);
	
// 	// int *k = (int *)malloc(trainingData->entityCount*sizeof(int));
// 	// e.findBestMacroThreshold(cpeEntityPairs,trainingData);
// 	// int *predictedEntityPairsLabel=e.findLabelsAndKValues(cpeMentions,3,trainingData,k);


// 	// for(int i=0;i<trainingData->entityCount;i++)
// 	// {
// 	// 	//cout<<k[i]<<"  "<<predictedEntityPairsLabel[i]<<endl;
// 	// }

	
// 	return 0;
// }
