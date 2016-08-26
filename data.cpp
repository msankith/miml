
#include"data.h"


Data::Data()
{

}
int probabilityChooser(double key,double *list,int start,int end,int count,int max);

void initializer(double *arr,int size,double val){
	for(int i=0;i<size;i++)
		arr[i]=val;
}

void Data::setMentionLabels(const double *kValues,const double *cpeMentions,double *yLabels) const
{
	int *mentionsPerEntityPair= this->mentionsPerEntityPairCount;
	srand (time(NULL));
	
	int max=0;
	for(int z=0;z<this->entityCount;z++){
		if(max<mentionsPerEntityPair[z])
			max=mentionsPerEntityPair[z];
	}
	int maxSize=max;
	double *tempArray =(double *)malloc(sizeof(double)*maxSize); // Size can be reduced. //Cummulative CPE ll be scored
	double *randomTillNow =(double *)malloc(sizeof(double)*maxSize); // to track which all selected
	double *itrIncrementor =(double *)malloc(sizeof(double)*maxSize); // for recaluclating the adjusted iterative pointer after removing the selected mention from the list 
	
	
	
	initializer(yLabels,mentionsCount,0);
	
	
	int mentionsTrack=0;
	for(int itrEntity=0;itrEntity<entityCount;itrEntity++)
	{
		initializer(tempArray,maxSize,0);
		initializer(randomTillNow,maxSize,0);
		initializer(itrIncrementor,maxSize,0);

		int itrMentions=0;
		int mentionsSize=mentionsPerEntityPair[itrEntity];
		//Cummulative CPE for every entity pair over all mentions
		tempArray[itrMentions]=cpeMentions[mentionsTrack];
		for(itrMentions=1;itrMentions<mentionsPerEntityPair[itrEntity];itrMentions++)
		{
			tempArray[itrMentions]=tempArray[itrMentions-1]+cpeMentions[mentionsTrack+itrMentions];
		}
		int k =(int) round( (kValues[itrEntity]*mentionsSize));
		if(kValues[itrEntity]==1){
		// cout<<endl<<"Mentions Track \t"<<mentionsTrack<<endl;
		// cout<<"Mentions on entity \t"<<mentionsSize<<"\tk to set\t"<<k<<endl;;
		//Set k mentions probablistically
			for(int set=0;set<mentionsSize;set++)
				yLabels[mentionsTrack+set]=1;
		}
		else 
		{
			for(int set=0;set<k;set++)
			{
				double randomNumber = ((double) rand() / (RAND_MAX))*tempArray[itrMentions-1];
				int ptr= probabilityChooser(randomNumber,tempArray,0,mentionsSize-1,0,mentionsSize);
				randomTillNow[ptr]=1;
				int mentionToSet=ptr+itrIncrementor[ptr];
				//cout<<mentionsTrack+mentionToSet<<"\t";
				yLabels[mentionsTrack+mentionToSet]=1;


				if(mentionToSet>mentionsPerEntityPair[itrEntity])
				{
					cout<<"Instances in entity pair "<<mentionsPerEntityPair[itrEntity]<<"\t"<<mentionToSet<<endl;
					cout<<"ERROR IN SETTING LATENT LABELS, out of bound. Data.cpp/setMentionLabels"<<endl;
					exit(0);
				}
				for(int temp=ptr;temp<mentionsPerEntityPair[itrEntity];temp++){
						itrIncrementor[temp]++;
				}
				mentionsSize--;
				// memset (tempArray, 0, sizeof (double) * maxSize);
				initializer(tempArray,maxSize,0);
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
		}
		mentionsTrack+=mentionsPerEntityPair[itrEntity]; // mentionsTrack ll point to iterator  of entity pairs first mention.	
	}
	free(tempArray);
	free(randomTillNow);
	free(itrIncrementor);
}


double Data::getWeightsOfCostSensitiveClassifier(int relationNumber) const
{
	double *yLabels= allLabels[relationNumber];
	int count=0;
	for(int i=0;i<entityCount;i++)
	{
		if(yLabels[i]==1)
			count++;
	}
	return (double)(count<10?50:count)/entityCount;
	// cout<<"Setting weighjts"<<endl;
	// *weight_label=(int *)malloc(sizeof(int)*2);
	// *weight=(double *)malloc(sizeof(double)*4);
	// cout<<"Weight initi"<<endl;
	// *weight_label[0]=0;
	// *weight_label[1]=1;
	// cout<<"Weight initi"<<endl;
	// *weight[0]=(double)(count<10?50:count)/entityCount;
	// cout<<"Weight initi"<<endl;
	// *weight[1]=0;
	// // *weight[1]=1-(double)(count<10?50:count)/entityCount;
	

	// // cout<<count<<"\tWeights are "<<weight[0]<<"\t"<<weight[1]<<endl;
	// cout<<"Weights set"<<endl;
}

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

void Data::myPrint(bool printAll=false) const
{
	cout<<"Number of mentions :" << mentionsCount<<endl;
	cout<<"Number of entity :" << entityCount<<endl;
	cout<<"Number of number of relations :" << numberOfRelations<<endl;
	cout<<"Number of features :" << prob.n<<endl;

	cout<<"Feature index "<<prob.x[18][21].index<<":"<<prob.x[18][21].value<<endl;
	cout<<"Feature index "<<prob.x[18][22].index<<":"<<prob.x[18][22].value<<endl;
	if(printAll)
	for(int i=0;i<mentionsCount;i++)
	{
		int j=0;
		while(prob.x[i][j].index!=-1)
		{
			if(j>0)
				cout<<" ";
			cout<<prob.x[i][j].index<<":"<<prob.x[i][j].value;
			j++;
		}
		cout<<endl;
	}

}



void Data::myPrint(double *array,int size) const
{
	for (int i=0;i<size;i++)
	{
		cout<<array[i]<<endl;
	}
}


