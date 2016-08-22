#include<iostream>
#include"config.h"
#include"evaluations.h"
#include"global.h"

using namespace std;
#define SIZE(A) (sizeof(A)/sizeof(*(A)))

Data *trainingData;

void print(Data *d)
{
	int i=0;
	cout<<"Entity Count   "<<d->entityCount<<endl;
	cout<<"Mentions Count   "<<d->mentionsCount<<endl;
	cout<<"Mentions per entity  "<<endl;
	while(i<d->entityCount)
	{
		cout<<d->mentionsPerEntityPairCount[i++]<<" ";
	}
	cout<<endl;

	cout<<"TRUE ENTITY LABELS"<<endl;
	i=0;
	while(i<d->entityCount)
	{
		cout<<d->trueEntityLabels[i++]<<" ";
	}
	cout<<endl;


	cout<<"-------"<<endl;
	i=0;
	while(i<d->mentionsCount)
		cout<<d->cpeMentions[i++] <<" ";

	cout<<endl<<endl<<"CPE MENTIONS"<<endl;
	i=0;
	int itr=0;
	while(i<d->entityCount)
	{
		int j=0;
		while(j<d->mentionsPerEntityPairCount[i])
		{
			cout<<d->cpeMentions[itr++]<<" ";
			j++	;
		}
		i++;
		cout<<"\t";
		//cout<<d->mentionsPerEntityPairCount[i++]<<" ";
	}
	
	cout<<endl;
	
}
int main()
{
	
	srand (time(NULL));

	// Data::entityAllLabels;// = (int *)malloc(sizeof(int *)* 1);
	Data randomData= Data("dataset/reidel_trainSVM.data");
	// print(&randomData);

	// Evaluation e = Evaluation();

	// double *cpeEntityPairs= e.getMaxCpePerEntityPair(&randomData);
	// double threshold= e.findBestMacroThreshold(cpeEntityPairs,&randomData);
	// double *k = e.getKForEntityPairs(&randomData,threshold);

	// double *sortedEntityPairs = e.sortArray(cpeEntityPairs,randomData.entityCount);
	// int i=0;
	// while(i<randomData.entityCount)
	// 	cout<<sortedEntityPairs[i++]<<" ";
	// cout<<endl;
	
	// cout<<"k values"<<endl;
	// i=0;
	// while(i<randomData.entityCount)
	// 	cout<<k[i++]<<" ";
	// cout<<endl;
	

	// cout<<"Best threshold "<< threshold<<"\t"<<endl<<endl<<endl;

	// int mentionsEntityPairCount[]={2,3,2};
	// int trueLabels[]={1,0,1};
	// double cpeMentions[]={1,2,1,8,4,6,7};

	// trainingData =new  Data(SIZE(cpeMentions),SIZE(mentionsEntityPairCount),cpeMentions,mentionsEntityPairCount,trueLabels);
	// Evaluation e = Evaluation();

	// //e.getMaxCpePerEntityPair(cpeMentions,mentionsEntityPairCount);
	// double *cpeEntityPairs=e.getMaxCpePerEntityPair(trainingData);
	
	// int *k = (int *)malloc(trainingData->entityCount*sizeof(int));
	// e.findBestMacroThreshold(cpeEntityPairs,trainingData);
	// int *predictedEntityPairsLabel=e.findLabelsAndKValues(cpeMentions,3,trainingData,k);


	// for(int i=0;i<trainingData->entityCount;i++)
	// {
	// 	//cout<<k[i]<<"  "<<predictedEntityPairsLabel[i]<<endl;
	// }

	
	return 0;
}
