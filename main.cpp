#include<iostream>
#include "data.h"
#include "config.h"
#include <cmath>
#include "linear.h"
#include "svm.h"
#include <cmath>
#include <sstream>
#include <stdlib.h>
#include "evaluations.h"
#include<fstream>
#include<string.h>

using namespace std;
#define MAXBUFLEN 1000000
#define INF HUGE_VAL

Data * loadData(string fname);
void coreLogic(int relationNumber,const Config *config,const Data *data,double *kValues,const Data *validDataset);
void intializeKValues(double *kValues,double *labels,double val,int size);	
void initialize(double *array,int size,double val);
double *getCpe(const struct model *relationModel,const Data *data,bool print);
double *getCpe(const struct svm_model *relationModel,const Data *data,bool print);
int isTrainingDataExist(const double *labels,int size)
{
	int count =0;
	for(int i=0;i<size;i++)
	{	if(labels[i]==1)
			count++;
	}
	return count;
}

std::string to_string(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}
void printToFile(double *data,int size,string fname,int relationNumber)
{
	std::ofstream ofs;
  	
  	string path = "temp/"+fname+"_"+to_string(relationNumber)+".txt";
  	ofs.open (path.c_str(), std::ofstream::out );
  	for(int i=0;i<size;i++)
  		ofs<<i<<"\t"<<data[i]<<endl;
  	
  	ofs.close();
}

void printToFile(double *data,int size,double threshold,const Data *dataset,string fname,int relationNumber)
{
	std::ofstream ofs;
  	
  	string path = "temp/"+fname+"_"+to_string(relationNumber)+".txt";
  	ofs.open (path.c_str(), std::ofstream::out );
  	for(int i=0;i<size;i++)
  		ofs<<i<<"\t"<<data[i]<<"\t"<<threshold<<"\t"<<dataset->allLabels[relationNumber][i]<<endl;
  	

  	ofs.close();
}

void myprint(double *data,int size)
{
	for(int i=0;i<size;i++)
		cout<<data[i]<<"\t";
	cout<<endl;
}


double *fScore_all;
double *threshold_all;

ofstream programOutput;



int main()
{
	Config config= Config();
	Data *dataset = loadData("dataset/training75");
		// Data *dataset= loadData("dataset/reidel_trainSVM.data");

	//Data *dataset = loadData("/home/ankith/learning/mtp/code/experiment_miml_c/code_experiments/dataset/reidel_trainSVM.data_bck_modifiedDataset_10_25_36_37");
	 Data *validDataset = loadData("dataset/valid25");
	string outputFile = config.outputFileDirectory+"programOutput";
	programOutput.open(outputFile.c_str(),std::fstream::out);
	fScore_all = (double *) malloc(sizeof(double)*dataset->numberOfRelations);
	threshold_all = (double *) malloc(sizeof(double)*dataset->numberOfRelations);
	
	initialize(fScore_all,dataset->numberOfRelations,0);
	initialize(threshold_all,dataset->numberOfRelations,0);
	 // int relationToConsider[]={0,9,12,15,20,24,30,31,35,36,42,43,44,45,49};
	// int relationToConsider[]={9};//,24,35,36};
	// int relationToConsider[]={24,30,31,35};
	
	// int relationNumber=24;
	  // for(int j=0;j<sizeof(relationToConsider)/sizeof(int);j++)
	 for(int relationNumber=0;relationNumber<dataset->numberOfRelations;relationNumber++)
	{
		  // int relationNumber=relationToConsider[j];
		cout<<endl<<"--------------------Current Relation Number "<<relationNumber<<" ---------------------"<<endl;
		programOutput<<endl<<"--------------------Current Relation Number "<<relationNumber<<" ---------------------"<<endl;
 		
		int count =isTrainingDataExist(dataset->allLabels[relationNumber],dataset->entityCount);
		cout<<"Relation Number "<<relationNumber+1<<"\t Labels Count\t"<<count<<endl;
		
		if(count ==0)
		{
			cout<<"Training Data doesnt not exist for label Number "<<relationNumber+1<<endl;
			programOutput<<"Training Data doesnt not exist for label Number "<<relationNumber+1<<endl;
			continue;
		}
		double *kValues = (double *)malloc(sizeof(double)*dataset->entityCount);
		coreLogic(relationNumber,&config,dataset,kValues,validDataset);
	}
	cout<<"*****End Of Program *****"<<endl;
	programOutput<<"*****End Of Program *****"<<endl;
	free(dataset->prob.x);
	programOutput.close();
	printToFile(fScore_all,dataset->numberOfRelations,"FSCORE",0);
	printToFile(threshold_all,dataset->numberOfRelations,"Threshold",0);

}

static void print_null(const char *s) {}
 void coreLogic(int relationNumber,const Config *config,const Data *data,double *kValues,const Data *validDataset)
 {
 	double *yLabels = (double *)malloc(sizeof(double)*data->mentionsCount);
 	double *cpeMentions=(double *) malloc(sizeof(double)*data->mentionsCount);
 	struct problem libProb = data->prob;
 	struct svm_problem libProb_svm = data->prob_svm;
 	
 	libProb.y = yLabels; 
 	libProb_svm.y=yLabels;
 	Evaluation eval = Evaluation();
	double threshold;
	struct model *relationModel;
	struct parameter param;
	struct svm_parameter param_svm;
	double bestC;
	double bestRate;
	double bestThreshold;
	double bestFscore;
	double bestWeight;
	struct model *bestModel;

	param= config->param;
	param_svm=config->param_svm;
	bestFscore=0;
	int bestEpoch=0;
	struct svm_model *relationModel_svm;
	struct svm_model *bestModel_svm;
	
	bool isSVM= config->isSVM;

		
	for(int itrKrange=0;config->k[itrKrange]!=0;itrKrange++)
 	{

 		intializeKValues(kValues,data->allLabels[relationNumber],config->k[itrKrange],data->entityCount);
 		initialize(cpeMentions,data->mentionsCount,0.5);
		
		for(double weight=config->minCost;weight<config->maxCost;weight+=config->stepSize)
		{	
			for(int epoch=0;epoch<config->numberOfEpochs;epoch++)
			{
				// printToFile(kValues,data->entityCount,"initial_Kvalues_"+to_string(epoch),relationNumber);
				data->setMentionLabels(kValues,cpeMentions,yLabels,relationNumber); // Should we reinitialize the CPE Mentions
				// double w = data->getWeightsOfCostSensitiveClassifier(relationNumber);
				if(config->findBestCosts)
				 {
				 	cout<<"weight\t"<<weight<<endl;
				 	// param.weight=(double *) malloc(sizeof(double)*4);
					 param.weight[0]=weight;
					 param.weight[1]=1.0-weight;
					 param.nr_weight=2;
					 cout<<"cost based learnner.Current Cost "<< weight<<endl;
				 }
				if(config->findBestC)
				{
					find_parameter_C(&libProb,&param,config->nr_folds,config->C_Min,config->C_Max,&bestC,&bestRate,data->mentionsPerEntityPairCount,data->entityCount,data->allLabels[relationNumber]);
					cout<<"Best C \t"<<bestC<<endl;
					param.C=bestC;
				}
				//param.C= 12;
				//cout<<param.eps<<"\t solver type"<<param.solver_type<<endl;
				set_print_string_function(&print_null);
				if(isSVM){
					cout<<"SVM TRAINING"<<endl;
					relationModel_svm=svm_train(&libProb_svm,&param_svm);
					cout<<"end of SVM"<<endl;
				}else
					relationModel= train(&libProb,&param);
				
				free(cpeMentions);
				if(isSVM)
					cpeMentions= getCpe(relationModel_svm,data,false); 
				else
					cpeMentions= getCpe(relationModel,data,false); 

				double *cpeEntityPairs = eval.getMaxCpePerEntityPair(data,cpeMentions);
				
				double tempfScore;
				double *cpeValidMentions;
				if(isSVM)
					cpeValidMentions= getCpe(relationModel_svm,validDataset,false);
				else
					cpeValidMentions= getCpe(relationModel,validDataset,false);
				threshold= eval.findBestMacroThreshold(cpeEntityPairs,data,relationNumber,&tempfScore,validDataset,cpeValidMentions);
				cout<<"Epoch "<<epoch<<"\t\tF-Score "<<tempfScore<<endl<<endl;
				if(tempfScore>=bestFscore)
				{
					bestFscore=tempfScore;
					bestThreshold=threshold;
					bestModel_svm=relationModel_svm;
					bestModel=relationModel;
					bestWeight=weight;
					bestEpoch=epoch;
				}
				free(kValues);
				
				kValues= eval.getKForEntityPairs(data,threshold,cpeMentions,relationNumber);
				free(cpeEntityPairs);
				free(cpeValidMentions);
			}
		}
	}	
	cout<<"Best EPoch\t"<<bestEpoch<<endl;
	cout<<"Best Validaton F Score "<<bestFscore<<"\t Best Threshold \t"<<bestThreshold<<endl;
	programOutput<<"Best EPoch\t"<<bestEpoch<<endl;
	programOutput<<"Best Validaton F Score "<<bestFscore<<"\t Best Threshold \t"<<bestThreshold<<endl;
	free(cpeMentions);
	cout<<"***************TESTING***************"<<endl;
	programOutput<<"***************TESTING***************"<<endl;
	
	Data *testingData = loadData("dataset/testSVM.pos_r.data");
	if(isSVM)
		cpeMentions = getCpe(bestModel_svm,testingData,false);
	else
		cpeMentions = getCpe(bestModel,testingData,false);
	double fScore= eval.getFScore(testingData,bestThreshold,cpeMentions,relationNumber,1/*beta*/); 
	cout<<"Best Threshold\t"<<bestThreshold<<"\tBest weight "<<bestWeight<<endl;
	cout<<"Relation Number \t"<<relationNumber<<"\t Fscore\t"<<fScore<<endl;
	programOutput<<"Best Threshold\t"<<bestThreshold<<"\tBest weight "<<bestWeight<<endl;
	programOutput<<"Relation Number \t"<<relationNumber<<"\t Fscore\t"<<fScore<<endl;
	
	fScore_all[relationNumber]=fScore;
	threshold_all[relationNumber]=threshold;
	cout<<"\n-----------END OF ONE RELATION--------------"<<endl;
	programOutput<<"\n-----------END OF ONE RELATION--------------"<<endl;
				
 	

 }


void intializeKValues(double *kValues,double *labels,double val,int size)
{
	for(int i=0;i<size;i++)
	{
		if(labels[i]==1)
			kValues[i]=val;
		else
			kValues[i]=0.0;
	}
}

void initialize(double *arr,int size,double val)
{
	for(int i=0;i<size;i++)
		arr[i]=val;
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
	struct svm_node **trainingMentionsFeatures_svm,*feature_svm;
	struct problem *trainingFeaturesPrb;
	struct svm_problem *trainingFeaturesPrb_svm;


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
	trainingMentionsFeatures_svm = (struct svm_node **) malloc(totFeature * sizeof(struct svm_node *));

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
			trainingMentionsFeatures_svm[featureNo] = (struct svm_node *) malloc((noActFeature+1) * sizeof(struct svm_node));
			
			//cout << "Total no of feature: " << noActFeature << endl ;
			for(j = 0; j < noActFeature; j++)
			{
				feature = (struct feature_node *) malloc(sizeof(feature_node));
				feature_svm = (struct svm_node *) malloc(sizeof(svm_node));
				
				fscanf(filePtr, "%s", line);
				stringstream fval(line);
				fval >> feature->index;
				feature_svm->index= feature->index;
			//	cout << "Featur index: " << feature->index << endl;
				if(feature->index > maxFeature)
					maxFeature = feature->index;
			//	cout << "Max Featur index: " << maxFeature << endl;
				if (fval.peek() == ':')
            		fval.ignore();
				fval >> feature->value;
				feature_svm->value=feature->value;
			//	cout << "Featur value: " << feature->value << endl;
				trainingMentionsFeatures[featureNo][j] = *feature;
				trainingMentionsFeatures_svm[featureNo][j] = *feature_svm;
				
				//cout << "Val for index: " << feature->index << " is: " << feature->value << " on iteration :" << featureNo  << " entity no: " << ite << endl;
			}	
			trainingMentionsFeatures[featureNo][j].index=-1;
			trainingMentionsFeatures[featureNo][j].value=0;
			
			trainingMentionsFeatures_svm[featureNo][j].index=-1;
			trainingMentionsFeatures_svm[featureNo][j].value=0;
			
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

	trainingFeaturesPrb_svm = (struct svm_problem *) malloc(sizeof(struct svm_problem));
	trainingFeaturesPrb_svm->l= totFeature;
	trainingFeaturesPrb_svm->x= trainingMentionsFeatures_svm;
	
	// cout << totFeature << " and " << featureNo << endl;
	 //double *opRes = (double *) calloc(totFeature, sizeof(double));
	 //trainingFeaturesPrb -> y = opRes;


	//dataLoad->featureVector = trainingMentionsFeatures;
	dataLoad->mentionsCount = totFeature;
	dataLoad->entityCount = entityPairNo;
	dataLoad->mentionsPerEntityPairCount=mentionsPerEntityPair;
	dataLoad->prob = *trainingFeaturesPrb;
	dataLoad->numberOfRelations=relationNo;
	dataLoad->allLabels=trainingLabelMatrix;
	dataLoad->prob_svm=*trainingFeaturesPrb_svm;
	/***********************************/

	// struct parameter param;

	// param.solver_type = L2R_L2LOSS_SVC_DUAL;
	// param.C = 1;
	// param.eps = INF; // see setting below
	// param.p = 0.1;
	// param.nr_weight = 0;
	// param.weight_label = NULL;
	// param.weight = NULL;
	// param.init_sol = NULL;

	/***********************************/


	//cout<<"Training"<<endl;
	return dataLoad;	

}

double *getCpe(const struct model *relationModel,const Data *data,bool print)
{
	double *prob_estimates=NULL;
	double *cpeMentions = (double *)malloc(sizeof(double)*data->prob.l);
	prob_estimates = (double *) malloc(relationModel->nr_class*sizeof(double));
	for(int itr=0;itr<data->prob.l;itr++) //prob.l gives number of mentions
	{	
		double predict_label = predict_probability(relationModel,data->prob.x[itr],prob_estimates);
		if(relationModel->label[0]==1)
			cpeMentions[itr]=prob_estimates[0];
		else
			cpeMentions[itr]=prob_estimates[1];
		if(print)
			cout<<itr<<"\tLabel \t "<<predict_label<<"\t" <<prob_estimates[0]<<'\t'<<prob_estimates[1]<<endl;
		if(cpeMentions[itr]<0)
		{
			cout<<"NEGATIVE CPE ---------- "<<endl;
			exit(0);
		}
		//cout<<cpeMentions[itr]<<"\t"<<prob_estimates[1]<<endl;
	}
	// /cout<<"-----"<<endl;
	return cpeMentions;
}

double *getCpe(const struct svm_model *relationModel,const Data *data,bool print)
{

	double *prob_estimates=NULL;
	double *cpeMentions = (double *)malloc(sizeof(double)*data->prob_svm.l);
	cout<<"SVM MODEL ESTIMATES"<<endl;
	prob_estimates = (double *) malloc(relationModel->nr_class*sizeof(double));
	cout<<"Number of examples "<<data->prob_svm.l<<endl;
	for(int itr=0;itr<data->prob_svm.l;itr++) //prob.l gives number of mentions
	{	
		double predict_label = svm_predict_probability(relationModel,data->prob_svm.x[itr],prob_estimates);
		if(relationModel->label[0]==1)
			cpeMentions[itr]=prob_estimates[0];
		else
			cpeMentions[itr]=prob_estimates[1];
		if(print)
			cout<<itr<<"\tLabel \t "<<predict_label<<"\t" <<prob_estimates[0]<<'\t'<<prob_estimates[1]<<endl;
		if(cpeMentions[itr]<0)
		{
			cout<<"NEGATIVE CPE ---------- "<<endl;
			exit(0);
		}
		//cout<<cpeMentions[itr]<<"\t"<<prob_estimates[1]<<endl;
	}
	cout<<"END OF CPE"<<endl;
	// /cout<<"-----"<<endl;
	return cpeMentions;
}