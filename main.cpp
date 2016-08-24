#include<iostream>
#include"config.h"
#include"evaluations.h"
#include"global.h"
#include "linear.h"
#include <cmath>
#include <sstream>
#include <stdlib.h>
#include<string.h>
#include "extraFunctions.h"

using namespace std;
#define SIZE(A) (sizeof(A)/sizeof(*(A)))

#define MAXBUFLEN 1000000
#define INF HUGE_VAL


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

void loadData(string fname,struct feature_node **vector,int *mentionsPerEntityPair,double **labelMatrix,int *entityPairsCount,int *mentionsCount);
void coreLogic(int relationNumber,Config *config,Data *data,double *kValues);
double *getCpe(struct model *relationModel,Data *data);
Data * loadData(string fname);

int main()
{
	Config config = Config();
	//loadData(config.trainingFileName,trainingMentionsFeatures,trainingMentionsPerEntityPair,trainingLabelMatrix,&trainingEntityPairsCount,&trainingMentionsCount);
	//loadData(config.testingFileName,testingMentionsFeatures,testingMentionsPerEntityPair,testingLabelMatrix);
	// int mentionsCount;
	// int entityCount;
	// int *mentionsPerEntityPairCount;
	// double *cpeMentions;
	// int numberOfRelations;
	
	// struct problem prob;


	Data *dataset = loadData("dataset/reidel_trainSVM.data_bck");
	cout<<"End of loading"<<endl;
	dataset->myPrint();
	//myPrint(trainingMentionsPerEntityPair,4);
	for(int relationNumber=0;relationNumber<dataset->numberOfRelations;relationNumber++)
	{
		double *kValues = (double *)malloc(sizeof(double)*dataset->entityCount);
		//Data *data = new Data(trainingMentionsFeatures,trainingLabelMatrix[relationNumber],trainingMentionsPerEntityPair,trainingEntityPairsCount,trainingMentionsCount); 
		//Data *data = new Data(relationNumber,dataset);
		// CPE Mentions should be intiailzed to constant value
		coreLogic(relationNumber,&config,dataset,kValues);
	}
	
}


void coreLogic(int relationNumber,Config *config,Data *data,double *kValues)
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
		double *cpeMentions=(double *) malloc(sizeof(double)*data->mentionsCount);
		memset(cpeMentions,0.5,sizeof(double)*data->mentionsCount);
		for(int epoch=0;epoch<config->numberOfEpochs;epoch++)
		{
			data->setMentionLabels(kValues,cpeMentions);
			relationModel= train(&data->prob,&config->param);
			cpeMentions= getCpe(relationModel,data); 
			double *cpeEntityPairs = eval.getMaxCpePerEntityPair(data,cpeMentions);
			threshold= eval.findBestMacroThreshold(cpeEntityPairs,data,relationNumber);
			kValues= eval.getKForEntityPairs(data,threshold,cpeMentions);
		}
		//struct Data *testingData = new Data(testingMentionsFeatures,testingLabelMatrix[relationNumber],testingMentionsPerEntityPair,testingEntityPairsCount,testingMentionsCount) ;
		cout<<"-------------END OF TRAINING------------  "<<endl;
		Data *testingData = loadData("dataset/reidel_trainSVM.data");
		cpeMentions = getCpe(relationModel,testingData);
		double fScore= eval.getFScore(testingData,threshold,cpeMentions,relationNumber); 
		cout<<"Relation Number \t"<<relationNumber<<fScore<<endl;
		if(fScore>bestFScore)
		{
			bestFScore=fScore;
			bestRelationModel=relationModel;
			bestThreshold=threshold;
			bestK=initialK;
		}
	}
}


void loadData(string fname,struct feature_node **trainingMentionsFeatures,int *mentionsPerEntityPair,double **labelMatrix,int *entityPairsCount,int *dataMentionsCount){
}


Data * loadData(string fname)
{
	Data *dataLoad = (Data *)malloc(sizeof(Data));
	FILE *filePtr;
	char line[MAXBUFLEN];
	int relationNo, entityPairNo,*mentionsPerEntityPair;
	double **trainingLabelMatrix;
	int i, j, k = 0, ite;
	int actReltnNo, entityCnt = 0, currActLabel, noFeatureVec, noActFeature, currActFeature;
	int featureIndex;
	double featureVal;
	long int totFeature = 0, maxFeature = 0, featureNo=0;

	struct feature_node **trainingMentionsFeatures, *feature;
	struct problem *trainingFeaturesPrb;


	//trainingMentionsFeatures = (struct feature_node **) malloc(sizeof(struct feature_node));


	filePtr = fopen(fname.c_str(), "r");

	if(filePtr != NULL)
		fscanf(filePtr, "%d", &entityPairNo);

	//cout << "No of Entity Pair: " << entityPairNo << endl;

	if(filePtr != NULL)
		fscanf(filePtr, "%d", &relationNo);

	// cout << "No of Relation: " << relationNo << endl;

	trainingLabelMatrix = (double **) malloc(relationNo * sizeof(double *));
	mentionsPerEntityPair= (int *) malloc(entityPairNo * sizeof(int));
	for(i = 0; i < relationNo; i++)
	{
		trainingLabelMatrix[i] = (double *) calloc(entityPairNo , sizeof(double));
	}

	/**********************************/
	// For finding the total mentions */
	//*********************************/
	for(ite = 0; ite < entityPairNo; ite++)
	{
		fscanf(filePtr, "%d", &actReltnNo);
		// cout << "[check]Number of relations active for entity pair " << ++k << " :" << actReltnNo << endl;
		for(i = 0; i < actReltnNo; i++)
		{
			fscanf(filePtr, "%d", &currActLabel);
			// cout << "[check]Relation no: " << currActLabel << " is active\n" ;
			//trainingLabelMatrix[i][currActLabel-1] = 1;
		}
		fscanf(filePtr, "%d", &noFeatureVec);
		totFeature = totFeature + noFeatureVec;
		mentionsPerEntityPair[ite]=noFeatureVec;
		// cout << "[check]Number of mentions for the entity pair: " << noFeatureVec << endl; 
		for(i = 0; i < noFeatureVec; i++)
		{
			fscanf(filePtr, "%d", &noActFeature);
			fgets(line, MAXBUFLEN, filePtr);
			// cout << "[CHECK]" <<line << endl;
		}
	}


	// cout << "Total number of feature vectors: " << totFeature << endl;
	//cout << "I am here/\n";

	trainingMentionsFeatures = (struct feature_node **) malloc(totFeature * sizeof(struct feature_node *));

	rewind(filePtr);

	if(fscanf(filePtr, "%d", &entityPairNo) == EOF)
		exit(0);

	//cout << "No of Entity Pair: " << entityPairNo << endl;

	if(fscanf(filePtr, "%d", &relationNo) == EOF)
		exit(0);

	//cout << "No of Relation: " << relationNo << endl;
	
	for(ite = 0; ite < entityPairNo; ite++)
	{
		fscanf(filePtr, "%d", &actReltnNo);
		// cout << "Number of relations active for entity pair " << ++k << " :" << actReltnNo << endl;
		for(i = 0; i < actReltnNo; i++)
		{
			fscanf(filePtr, "%d", &currActLabel);
			// cout << "Relation no: " << currActLabel << " is active\n" ;
			trainingLabelMatrix[currActLabel-1][ite] = 1;
		}
		fscanf(filePtr, "%d", &noFeatureVec);
		// cout << "Number of mentions for the entity pair: " << noFeatureVec << endl; 
		for(i = 0; i < noFeatureVec; i++)
		{
			fscanf(filePtr, "%d", &noActFeature);
			trainingMentionsFeatures[featureNo] = (struct feature_node *) malloc((noActFeature+1) * sizeof(struct feature_node));
			//cout << "Total no of feature: " << noActFeature << endl ;
			for(j = 0; j < noActFeature; j++)
			{
				feature = (struct feature_node *) malloc(sizeof(feature_node));
				fscanf(filePtr, "%s", line);
				stringstream fval(line);
				fval >> feature->index;
			//	cout << "Featur index: " << feature->index << endl;
				if(feature->index > maxFeature)
					maxFeature = feature->index;
			//	cout << "Max Featur index: " << maxFeature << endl;
				if (fval.peek() == ':')
            		fval.ignore();
				fval >> feature->value;
			//	cout << "Featur value: " << feature->value << endl;
				trainingMentionsFeatures[featureNo][j] = *feature;
				//cout << "Val for index: " << feature->index << " is: " << feature->value << " on iteration :" << featureNo  << " entity no: " << ite << endl;
			}	
			trainingMentionsFeatures[featureNo][j].index=-1;
			trainingMentionsFeatures[featureNo][j].value=0;
			
			featureNo++;
			//fscanf(filePtr, "%s", line);
			//cout << "Line: " << line << endl;
		}
	}

	/*

double **featureVector;
	double *trueEntityLabels;

	int mentionsCount;
	int entityCount;
	int *mentionsPerEntityPairCount;
	double *cpeMentions;
	
	struct problem prob;
	*/

	trainingFeaturesPrb = (struct problem *) malloc(sizeof(struct problem));
	trainingFeaturesPrb->l = totFeature;
	trainingFeaturesPrb->n = maxFeature;
	trainingFeaturesPrb->bias = 0;
	trainingFeaturesPrb-> x = trainingMentionsFeatures;
	// cout << totFeature << " and " << featureNo << endl;
	double *opRes = (double *) calloc(totFeature, sizeof(double));
	trainingFeaturesPrb -> y = opRes;


	//dataLoad->featureVector = trainingMentionsFeatures;
	dataLoad->mentionsCount = totFeature;
	dataLoad->entityCount = entityPairNo;
	dataLoad->mentionsPerEntityPairCount=mentionsPerEntityPair;
	dataLoad->prob = *trainingFeaturesPrb;
	dataLoad->numberOfRelations=relationNo;
	dataLoad->allLabels=trainingLabelMatrix;
	/***********************************/

	struct parameter param;

	param.solver_type = L2R_L2LOSS_SVC_DUAL;
	param.C = 1;
	param.eps = INF; // see setting below
	param.p = 0.1;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	param.init_sol = NULL;

	/***********************************/


	cout<<"Training"<<endl;
	return dataLoad;	

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
