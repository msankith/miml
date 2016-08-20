#include<iostream>
#include"config.h"
#include"evaluations.h"
#include"global.h"

using namespace std;
#define SIZE(A) (sizeof(A)/sizeof(*(A)))

Data *trainingData;
int main()
{
	
	int mentionsEntityPairCount[]={2,3,2};
	int trueLabels[]={1,0,1};
	double cpeMentions[]={1,2,1,8,4,6,7};

	trainingData =new  Data(SIZE(cpeMentions),SIZE(mentionsEntityPairCount),cpeMentions,mentionsEntityPairCount,trueLabels);
	Evaluation e = Evaluation();

	//e.getMaxCpePerEntityPair(cpeMentions,mentionsEntityPairCount);
	double *cpeEntityPairs=e.getMaxCpePerEntityPair(trainingData);
	
	int *k = (int *)malloc(trainingData->entityCount*sizeof(int));
	e.findBestMacroThreshold(cpeEntityPairs,trainingData);
	int *predictedEntityPairsLabel=e.findLabelsAndKValues(cpeMentions,3,trainingData,k);


	for(int i=0;i<trainingData->entityCount;i++)
	{
		//cout<<k[i]<<"  "<<predictedEntityPairsLabel[i]<<endl;
	}

	
	return 0;
}
