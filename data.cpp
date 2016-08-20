#include"data.h"
#include<stdlib.h>

using namespace std;


Data::Data(int mentionsCount,int EntityCount,double* cpeMentions, int *mentionsEntityPairCount,int *trueLabels)
{

	//cout<<mentionsCount2<<endl;
	this->mentionsCount= mentionsCount;
	this->entityCount= EntityCount;
	
	this->mentionsPerEntityPairCount = mentionsEntityPairCount;
	this->cpeMentions=cpeMentions;
	this->trueEntityLabels= trueLabels;
	// testingMentionsCount;
	// testingEntityCount;
	// testingMentionsPerEntityPairCount;
}