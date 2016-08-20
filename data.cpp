#include"data.h"
#include<stdlib.h>

using namespace std;


Data::Data(int mentionsCount,int EntityCount,double* cpeMentions, int *mentionsEntityPairCount,int *trueLabels)
{

	this->mentionsCount= mentionsCount;
	this->entityCount= EntityCount;
	
	this->mentionsPerEntityPairCount = mentionsEntityPairCount;
	this->cpeMentions=cpeMentions;
	this->trueEntityLabels= trueLabels;
}


int randomNumber(int minValue,int maxValue)
{
	//minValue=minValue==0?1:minValue;
	return rand()%(maxValue-minValue)+ minValue;
}

Data::Data()
{

	cout<<"Generating random data"<<endl;
	mentionsCount=randomNumber(30,40);
	entityCount=randomNumber(7,14);
	mentionsPerEntityPairCount= (int *)malloc(sizeof(int)*entityCount);
	trueEntityLabels=(int *)malloc(sizeof(int)*entityCount);
	cpeMentions=(double *)malloc(sizeof(double)*mentionsCount);
	int sum=0;
	int i;
	for(i=0;i<entityCount-1;i++)
	{
		mentionsPerEntityPairCount[i]=randomNumber((mentionsCount/entityCount -2)==0?1:mentionsCount/entityCount -2,mentionsCount/entityCount +2);
		sum+=mentionsPerEntityPairCount[i];
	}
	mentionsPerEntityPairCount[i]= mentionsCount-sum;
	for(i=0;i<entityCount;i++)
		trueEntityLabels[i]=randomNumber(0,2);

	for(i=0;i<mentionsCount;i++)
		cpeMentions[i]=randomNumber(1,100);

}