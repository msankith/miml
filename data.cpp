#include"data.h"
#include<stdlib.h>
#include <fstream>
#include <cstring>
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
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


static char* line;
static int max_line_len=1024;
// static char* readline(ifstream *input)
// {
// 	int len;

// 	if(fgets(line,max_line_len,input) == NULL)
// 		return NULL;

// 	while(strrchr(line,'\n') == NULL)
// 	{
// 		max_line_len *= 2;
// 		line = (char *) realloc(line,max_line_len);
// 		len = (int) strlen(line);
// 		if(fgets(line+len,max_line_len-len,input) == NULL)
// 			break;
// 	}
// 	return line;
// }

Data::Data(string fname)
{
	ifstream inFile;
    inFile.open(fname.c_str());
    string s;
    int numberOfEntites,numberOfRelations;
	inFile>>numberOfEntites;
	inFile>>numberOfRelations;
	cout<<numberOfEntites<<"=========="<<numberOfRelations<<endl<<endl;

	entityAllLabels=(int **)malloc(sizeof(int **)*numberOfRelations);

	for(int i=0;i<numberOfRelations;i++)
		entityAllLabels[i]=(int *)malloc(sizeof(int *)*numberOfEntites);

	// int relations;

	while(!inFile.eof())
	{
		int relations;
		// getline(inFile,s);
		// cout<<"----"<<s<<endl;
		inFile>>relations;	
		cout<<"Number of relations "<<relations<<endl;
		for(int temp=0;temp<relations;temp++)
		{
			int rel;
			inFile>>rel;
			//cout<<rel;

		}
		int mentions;
		inFile>>mentions;
		cout<<"Number of mentions "<<mentions<<endl;
		for(int temp=0; temp<mentions;temp++)
		{

			string men;
			getline(inFile,s);
		
			//inFile>>men;
			cout<<s<<endl;
		}
		cout<<s<<endl;
	}
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