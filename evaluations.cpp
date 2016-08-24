#include"evaluations.h"
#include <algorithm>

using namespace std;

Evaluation::Evaluation()
{

}


double *  Evaluation::getMaxCpePerEntityPair(const Data *data,double *cpeMentions)
{

	double *cpeEntityPairs= (double *) malloc(data->entityCount*sizeof(double));
	int mentionsIterator=0;
	for(int entityPairsIterator=0; entityPairsIterator<data->entityCount ;  entityPairsIterator++)
	{
		int iterator=0;
		double maxCpe=0;
		while(iterator<data->mentionsPerEntityPairCount[entityPairsIterator])
		{
		//	cout<<data->cpeMentions[mentionsIterator]<<endl;
			if(maxCpe<cpeMentions[mentionsIterator])
				maxCpe=cpeMentions[mentionsIterator];
			mentionsIterator++;
			iterator++;
		}
		cpeEntityPairs[entityPairsIterator]=maxCpe;
		//cout<<endl<<endl;
	}	
	return cpeEntityPairs;
}


double * Evaluation::sortArray(const double *cpeEntityPairs,int SIZE)
{
	double *sortedCpeEntityPairs=(double * ) malloc((SIZE)*sizeof(double));
	std::copy(cpeEntityPairs,cpeEntityPairs + SIZE, sortedCpeEntityPairs);
	sort(sortedCpeEntityPairs, sortedCpeEntityPairs + SIZE);
	return sortedCpeEntityPairs;
}


double Evaluation::findBestMacroThreshold(const double *cpeEntityPairs,const Data *data,int relationNumber)
{
	double *thresholdValues= (double * ) malloc((data->entityCount + 1)*sizeof(double)); //+1 coz to include both borders

	double *sortedCpeEntityPairs= sortArray(cpeEntityPairs,data->entityCount);

	int entityPairsIterator=0;
	thresholdValues[entityPairsIterator]=sortedCpeEntityPairs[0]-0.01;
	for(entityPairsIterator=1; entityPairsIterator<data->entityCount ;  entityPairsIterator++)
	{
		thresholdValues[entityPairsIterator]= (sortedCpeEntityPairs[entityPairsIterator-1]+sortedCpeEntityPairs[entityPairsIterator])/2;
	}
	thresholdValues[entityPairsIterator]= sortedCpeEntityPairs[entityPairsIterator -1]+0.01;

	double maxFscore=0;
	double bestThreshold=0;
	for(int iterator=0;iterator<data->entityCount+1;iterator++)
	{
		double *predictedEntityPairsLabel = findLabelsBasedOnEntity(cpeEntityPairs,thresholdValues[iterator],data);
		double tempFscore=getFscore(predictedEntityPairsLabel,data->allLabels[relationNumber],data);
		if(tempFscore>maxFscore)
		{
			maxFscore=tempFscore;
			bestThreshold= thresholdValues[iterator];
		}
	}

	return bestThreshold;
}


double * Evaluation::findLabelsBasedOnEntity(const double *cpeMentions,double threshold,const Data *data)
{
	double *entityPairsLabel = (double * ) malloc(data->entityCount*sizeof(double));
	int mentionsIterator=0;
	for(int entityPairsIterator=0; entityPairsIterator<data->entityCount ;  entityPairsIterator++)
	{
	
		if(cpeMentions[entityPairsIterator]>threshold)
			entityPairsLabel[entityPairsIterator]=1;
		else
			entityPairsLabel[entityPairsIterator]=0;
	}
	return entityPairsLabel;
}

double Evaluation::getFscore(const double *predictedEntityLabels,const double *entityLabels,const Data *data)
{
	double precision;
	double recall;
	double beta=0.5;
	double betaSquare=beta*beta;

	//cout<<"get F scor fundtion"<<endl;
	int TP=0;
	int FP=0;
	int FN=0;
	for(int entityPairsIterator=0; entityPairsIterator<data->entityCount ;  entityPairsIterator++)
	{
		
		if(predictedEntityLabels[entityPairsIterator]==1 &&entityLabels[entityPairsIterator]==1)
		{
			TP++;
		}else if(predictedEntityLabels[entityPairsIterator]==1 &&entityLabels[entityPairsIterator]==0)
		{
			FP++;
		}else if(predictedEntityLabels[entityPairsIterator]==0 &&entityLabels[entityPairsIterator]==1)
		{
			FN++;
		}
	}
	double fvalue=0;
	
	if(TP==0)
		return fvalue;
	precision= (double)TP/(TP+FP);
	recall= (double)TP/(TP+FN);
	
	if(precision>0 && recall>0) 
		fvalue=(precision*recall)/(precision+recall);

	cout<<"Fscore " <<fvalue<<endl;
	return fvalue;

}

double Evaluation::getFScore(Data *data,double threshold,double *cpeMentions,int relationNumber)
{
	double *entityCpeMentions = getMaxCpePerEntityPair(data,cpeMentions);	
	//double *predictedEntityLabels = findLabelsBasedOnEntity(entityCpeMentions,threshold,data);
	double *predictedEntityPairsLabel = findLabelsBasedOnEntity(entityCpeMentions,threshold,data);
	double tempFscore=getFscore(predictedEntityPairsLabel,data->allLabels[relationNumber],data); ///

	return tempFscore;
	///return getFScore(predictedEntityLabels,data->trueEntityLabels,data);

}



double * Evaluation::getKForEntityPairs(const Data *data,double threshold,double *cpeMentions)
{
	//double *cpeMentions= data->cpeMentions;
	double *k = (double * ) malloc(data->entityCount*sizeof(double));
	int mentionsIterator=0;
	for(int entityPairsIterator=0; entityPairsIterator<data->entityCount ;  entityPairsIterator++)
	{
		int iterator=0;
		int ktemp=0;

		while(iterator<data->mentionsPerEntityPairCount[entityPairsIterator])
		{
			if(cpeMentions[mentionsIterator]>threshold)
			{
				ktemp++;
			}
			iterator++;
			mentionsIterator++;
		}
		//cout<< ktemp<<"  "<<data->mentionsPerEntityPairCount[entityPairsIterator]<<endl;
		k[entityPairsIterator]=(double)ktemp/data->mentionsPerEntityPairCount[entityPairsIterator];	
	}
	return k;
}


double * Evaluation::findLabelsBasedOnMentions(const double *cpeMentions,double threshold,const Data *data)
{
	double *entityPairsLabel = (double * ) malloc(data->entityCount*sizeof(double));
	int mentionsIterator=0;
	for(int entityPairsIterator=0; entityPairsIterator<data->entityCount ;  entityPairsIterator++)
	{
		int iterator=0;
		int ktemp=0;

		while(iterator<data->mentionsPerEntityPairCount[entityPairsIterator])
		{
			if(cpeMentions[mentionsIterator]>threshold)
			{
				ktemp++;
			}
			iterator++;
			mentionsIterator++;
		}
		if(ktemp>0)
			entityPairsLabel[entityPairsIterator]=1;
		else
			entityPairsLabel[entityPairsIterator]=0;
	}
	

	return entityPairsLabel;
}




int * Evaluation::findLabelsAndKValues(const double *cpeMentions,double threshold,const Data *data,int *k)
{
	int *mentionsLatentLabel = (int * ) malloc(data->mentionsCount*sizeof(int));
	int *entityPairsLabel = (int * ) malloc(data->entityCount*sizeof(int));
	//*k = (int * ) malloc(sizeof(data->entityCount)*sizeof(int));
	int mentionsIterator=0;
	for(int entityPairsIterator=0; entityPairsIterator<data->entityCount ;  entityPairsIterator++)
	{
		int iterator=0;
		int ktemp=0;

		while(iterator<data->mentionsPerEntityPairCount[entityPairsIterator])
		{
			if(cpeMentions[mentionsIterator]>threshold)
			{
				ktemp++;
			}
			iterator++;
			mentionsIterator++;
		}
		k[entityPairsIterator]=ktemp;
		if(ktemp>0)
			entityPairsLabel[entityPairsIterator]=1;
	}
	

	return entityPairsLabel;
}





double Evaluation::findBestMacroThreshold_BasedOnMention(double *cpeEntityPairs,Data *data,int relationNumber)
{
	double *thresholdValues= (double * ) malloc((data->entityCount + 1)*sizeof(double)); //+1 coz to include both borders

	double *sortedCpeEntityPairs= sortArray(cpeEntityPairs,data->entityCount);

	int entityPairsIterator=0;
	thresholdValues[entityPairsIterator]=sortedCpeEntityPairs[0]-0.01;
	for(entityPairsIterator=1; entityPairsIterator<data->entityCount ;  entityPairsIterator++)
	{
		thresholdValues[entityPairsIterator]= (sortedCpeEntityPairs[entityPairsIterator-1]+sortedCpeEntityPairs[entityPairsIterator])/2;
	}
	thresholdValues[entityPairsIterator]= sortedCpeEntityPairs[entityPairsIterator -1]+0.01;

	double maxFscore=0;
	double bestThreshold=0;
	for(int iterator=0;iterator<data->entityCount+1;iterator++)
	{
		double *cpeMentions;
		double *predictedEntityPairsLabel = findLabelsBasedOnMentions(cpeMentions,thresholdValues[iterator],data);
		cout<<"ANKITHHHHHHHHHHHH CPE MENTIONS NOT SET"<<endl;
		exit(0);
		double tempFscore=getFscore(predictedEntityPairsLabel,data->allLabels[relationNumber],data);
		if(tempFscore>maxFscore)
		{
			maxFscore=tempFscore;
			bestThreshold= thresholdValues[iterator];
		}
	}

	return bestThreshold;
}



