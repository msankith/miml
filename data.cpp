#include"data.h"
#include<stdlib.h>
#include <fstream>
#include <cstring>
#include <string.h>
#include<cmath>

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
using namespace std;




void Data::myPrint()
{

// int mentionsCount;
// 	int entityCount=;
// 	int *mentionsPerEntityPairCount;
// 	double *cpeMentions;
// 	int numberOfRelations;
	
// 	struct problem prob;



	cout<<"Number of mentions :" << mentionsCount<<endl;
	cout<<"Number of entity :" << entityCount<<endl;
	cout<<"Number of number of relations :" << numberOfRelations<<endl;
	cout<<"Number of features :" << prob.n<<endl;

	cout<<"Feature index "<<prob.x[18][21].index<<":"<<prob.x[18][21].value<<endl;
	cout<<"Feature index "<<prob.x[18][22].index<<":"<<prob.x[18][22].value<<endl;

}



Data::Data(int relationNumber,const Data *data)
{
	this-> mentionsCount= data->mentionsCount;
	this->entityCount= data->entityCount;
	this->mentionsPerEntityPairCount;
}

Data::Data(struct feature_node **features,double *labels,int *mentionsCountPerEntityPair,int pairsCount,int mentionsCount)
{
	this->prob.x= features;
	this->mentionsPerEntityPairCount=mentionsCountPerEntityPair;
	//this->trueEntityLabels=labels;
	this->entityCount=entityCount;
	this->mentionsCount=mentionsCount;

	this->prob.y = (double *)malloc(sizeof(double)*mentionsCount);
	//this->cpeMentions=(double *)malloc(sizeof(double)*mentionsCount);
	//memset (this->cpeMentions,0.5,sizeof(double)*mentionsCount);

//CPE Mentions should be intiailzed to constant value
}

// Data::Data(int mentionsCount,int EntityCount,double* cpeMentions, int *mentionsEntityPairCount,int *trueLabels)
// {

// 	this->mentionsCount= mentionsCount;
// 	this->entityCount= EntityCount;
	
// 	this->mentionsPerEntityPairCount = mentionsEntityPairCount;
// 	this->cpeMentions=cpeMentions;
// 	this->trueEntityLabels= trueLabels;
// }


int probabilityChooser(double key,double *list,int start,int end,int count,int max)
{
	int mid=(start+end)/2;
	if(count++>10)
		exit;
	if(start>=end){
	 	
	 	return start;
	 }
	else if(mid>0 && list[mid-1]<key && key <=list[mid] )
	{
		
		return mid;
	}else 
	if(key<list[mid])
		return probabilityChooser(key,list,start,mid-1,count++,max);
	else 
		return probabilityChooser(key,list,mid+1,end,count++,max);
}

//This function can be made more effective
void Data::setMentionLabels(double *kValues,double *cpeMentions)
{
	int *mentionsPerEntityPair= this->mentionsPerEntityPairCount;
	srand (time(NULL));
	int maxSize=3000;
	double *tempArray =(double *)malloc(sizeof(double)*maxSize); // Size can be reduced. //Cummulative CPE ll be scored
	double *randomTillNow =(double *)malloc(sizeof(double)*maxSize); // to track which all selected
	double *itrIncrementor =(double *)malloc(sizeof(double)*maxSize); // for recaluclating the adjusted iterative pointer after removing the selected mention from the list 
	
	
	memset (prob.y,0,sizeof(double)*mentionsCount);

	int mentionsTrack=0;
	for(int itrEntity=0;itrEntity<entityCount;itrEntity++)
	{
		memset (tempArray, 0, sizeof (double) * maxSize);
		memset (randomTillNow, 0, sizeof (double) * maxSize);
		memset (itrIncrementor, 0, sizeof (double) * maxSize);

		int itrMentions=0;
		int mentionsSize=mentionsPerEntityPair[itrEntity];
		//Cummulative CPE for every entity pair over all mentions
		tempArray[itrMentions]=cpeMentions[mentionsTrack];
		for(itrMentions=1;itrMentions<mentionsPerEntityPair[itrEntity];itrMentions++)
		{
			tempArray[itrMentions]=tempArray[itrMentions-1]+cpeMentions[mentionsTrack+itrMentions];
		}
		int k =(int) round( (kValues[itrEntity]*mentionsSize));

		//Set k mentions probablistically
		for(int set=0;set<k;set++)
		{
			double randomNumber = ((double) rand() / (RAND_MAX))*tempArray[itrMentions-1];
			int ptr= probabilityChooser(randomNumber,tempArray,0,mentionsSize-1,0,mentionsSize);
			randomTillNow[ptr]=1;
			int mentionToSet=ptr+itrIncrementor[ptr];
			
			prob.y[mentionToSet]=1;


			if(mentionToSet>mentionsPerEntityPair[itrEntity])
			{
				cout<<"ERROR IN SETTING LATENT LABELS, out of bound"<<endl;
				exit(0);
			}
			for(int temp=ptr;temp<mentionsPerEntityPair[itrEntity];temp++){
					itrIncrementor[temp]++;
			}
			mentionsSize--;
			memset (tempArray, 0, sizeof (double) * maxSize);
			for(int temp=0,itr=0;temp<mentionsPerEntityPair[itrEntity];temp++)
			{

				if(randomTillNow[temp]==1) //If mention already selected , skip 
					continue;
				if(itr==0)
					tempArray[itr]=cpeMentions[mentionsTrack+temp];
				else
					tempArray[itr]=tempArray[itr-1]+cpeMentions[mentionsTrack+temp];
				itr++;
			}		
		}
		mentionsTrack+=mentionsPerEntityPair[itrEntity]; // mentionsTrack ll point to iterator  of entity pairs first mention.		
	}
}

int randomNumber(int minValue,int maxValue)
{
	//minValue=minValue==0?1:minValue;
	return rand()%(maxValue-minValue)+ minValue;
}


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


static char *line = NULL;
static int max_line_len;

static char* readline(FILE *input)
{
	int len;

	if(fgets(line,max_line_len,input) == NULL)
		return NULL;

	while(strrchr(line,'\n') == NULL)
	{
		max_line_len *= 2;
		line = (char *) realloc(line,max_line_len);
		len = (int) strlen(line);
		if(fgets(line+len,max_line_len-len,input) == NULL)
			break;
	}
	return line;
}

Data::Data(string fname)
{
	FILE *input;
	ofstream entityMentionsCountFile,entityLabelsFile,mentionsFeatureFile;
	string filepath="/home/ankith/learning/mtp/code/miml_c/dataset/reidel_";
	string temp=filepath+"mentionsPerEntity";
	entityMentionsCountFile.open(temp.c_str());
	temp=filepath+"entityLabels";
	entityLabelsFile.open(temp.c_str());
	temp=filepath+"mentionsFeature";
	mentionsFeatureFile.open(temp.c_str());
	//

	input = fopen(fname.c_str(),"r");
		int inst_max_index;
		max_line_len = 1024;
		line = (char *)malloc(max_line_len*sizeof(char));
		int totalEntityPairs=atoi(readline(input));
		cout<<"Number of Entity Pairs "<<totalEntityPairs<<endl;
		
		int totalRelations=atoi(readline(input));
		cout<<"Number of Relations "<<totalRelations<<endl;
		bool firstLine=false;
		while(1)
		{
					inst_max_index = 0; // strtol gives 0 if wrong format
					line = readline(input);
					if(line==NULL)
						break;
					int relation = atoi(line);
					
					//cout<<"Number of Relations "<<relation<<endl;
					if(firstLine)
							entityLabelsFile<<"\n";
						
					for(int i=0;i<relation;i++)
					{
						int label = atoi(readline(input));
						//cout<<"--Label--"<<label<<endl;
						if(i==0)
							entityLabelsFile<<label;
						else
							entityLabelsFile<<"\t"<<label;
						
					}

					int mentions = atoi(readline(input));
					//	cout<<"Number of mentions "<<mentions<<endl;
						if(firstLine)
							entityMentionsCountFile<<"\n";
						entityMentionsCountFile<<mentions;

					for(int i=0;i<mentions;i++)
					{
						//cout<<"--men feat--"<<readline(input)<<endl;
						//string feat(readline(input));
						char *p;
    					p = strtok(readline(input), "\t");
						p = strtok(NULL, ",");
					    if(firstLine)
					    	mentionsFeatureFile<<"\n";
					    mentionsFeatureFile<<p;

						// mentionsFeatureFile<<feat;
						// cout<<feat.split("\t",2)<<endl;
					}
					firstLine=true;
					// prob.x[i] = &x_space[j];
					// label = strtok(line," \t\n");
					// if(label == NULL) // empty line
					// 	exit_input_error(i+1);

					// prob.y[i] = strtod(label,&endptr);
					// if(endptr == label || *endptr != '\0')
					// 	exit_input_error(i+1);

					// while(1)
					// {
					// 	idx = strtok(NULL,":");
					// 	val = strtok(NULL," \t");

					// 	if(val == NULL)
					// 		break;

					// 	errno = 0;
					// 	x_space[j].index = (int) strtol(idx,&endptr,10);
					// 	if(endptr == idx || errno != 0 || *endptr != '\0' || x_space[j].index <= inst_max_index)
					// 		exit_input_error(i+1);
					// 	else
					// 		inst_max_index = x_space[j].index;

					// 	errno = 0;
					// 	x_space[j].value = strtod(val,&endptr);
					// 	if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
					// 		exit_input_error(i+1);

					// 	++j;
					// }

					// if(inst_max_index > max_index)
					// 	max_index = inst_max_index;

					// if(prob.bias >= 0)
					// 	x_space[j++].value = prob.bias;

					// x_space[j++].index = -1;
		}
		
entityMentionsCountFile.close();
mentionsFeatureFile.close();
entityLabelsFile.close();
}

// Data::Data(string fname,int a)
// {
// 	ifstream inFile;
//     inFile.open(fname.c_str());
//     string s;
//     int numberOfEntites,numberOfRelations;
// 	inFile>>numberOfEntites;
// 	inFile>>numberOfRelations;
// 	cout<<numberOfEntites<<"=========="<<numberOfRelations<<endl<<endl;

// 	entityAllLabels=(int **)malloc(sizeof(int **)*numberOfRelations);

// 	for(int i=0;i<numberOfRelations;i++)
// 		entityAllLabels[i]=(int *)malloc(sizeof(int *)*numberOfEntites);

// 	// int relations;

// 	while(!inFile.eof())
// 	{
// 		int relations;
// 		// getline(inFile,s);
// 		// cout<<"----"<<s<<endl;
// 		inFile>>relations;	
// 		cout<<"Number of relations "<<relations<<endl;
// 		for(int temp=0;temp<relations;temp++)
// 		{
// 			int rel;
// 			inFile>>rel;
// 			//cout<<rel;

// 		}
// 		int mentions;
// 		inFile>>mentions;
// 		cout<<"Number of mentions "<<mentions<<endl;
// 		for(int temp=0; temp<mentions;temp++)
// 		{

// 			string men;
// 			getline(inFile,s);
		
// 			//inFile>>men;
// 			cout<<s<<endl;
// 		}
// 		cout<<s<<endl;
// 	}
// }

Data::Data()
{

	cout<<"Generating random data"<<endl;
	mentionsCount=randomNumber(30,40);
	entityCount=randomNumber(7,14);
	mentionsPerEntityPairCount= (int *)malloc(sizeof(int)*entityCount);
	double *trueEntityLabels=(double *)malloc(sizeof(double)*entityCount);
	
	double * cpeMentions=(double *)malloc(sizeof(double)*mentionsCount);
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
