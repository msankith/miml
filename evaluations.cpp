#include"evaluations.h"
#include <algorithm>

using namespace std;

Evaluation::Evaluation()
{

}

double *  Evaluation::getMaxCpePerEntityPair(double *cpeMentions,int *mentionsEntityPairCount)
{

}

double *  Evaluation::getMaxCpePerEntityPair(Data *data)
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
			if(maxCpe<data->cpeMentions[mentionsIterator])
				maxCpe=data->cpeMentions[mentionsIterator];
			mentionsIterator++;
			iterator++;
		}
		cpeEntityPairs[entityPairsIterator]=maxCpe;
		//cout<<endl<<endl;
	}	
	return cpeEntityPairs;
}


double * Evaluation::sortArray(double *cpeEntityPairs,int SIZE)
{
	double *sortedCpeEntityPairs=(double * ) malloc((SIZE)*sizeof(double));
	std::copy(cpeEntityPairs,cpeEntityPairs + SIZE, sortedCpeEntityPairs);
	sort(sortedCpeEntityPairs, sortedCpeEntityPairs + SIZE);
	return sortedCpeEntityPairs;
}


double Evaluation::findBestMacroThreshold(double *cpeEntityPairs,Data *data)
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
		int *predictedEntityPairsLabel = findLabelsBasedOnEntity(cpeEntityPairs,thresholdValues[iterator],data);
		double tempFscore=getFscore(predictedEntityPairsLabel,data->trueEntityLabels,data);
		if(tempFscore>maxFscore)
		{
			maxFscore=tempFscore;
			bestThreshold= thresholdValues[iterator];
		}
	}

	return bestThreshold;
}

double Evaluation::findBestMacroThreshold_BasedOnMention(double *cpeEntityPairs,Data *data)
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
		int *predictedEntityPairsLabel = findLabelsBasedOnMentions(data->cpeMentions,thresholdValues[iterator],data);
		double tempFscore=getFscore(predictedEntityPairsLabel,data->trueEntityLabels,data);
		if(tempFscore>maxFscore)
		{
			maxFscore=tempFscore;
			bestThreshold= thresholdValues[iterator];
		}
	}

	return bestThreshold;
}




int * Evaluation::findLabelsAndKValues(double *cpeMentions,double threshold,Data *data,int *k)
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

double * Evaluation::getKForEntityPairs(double *cpeMentions,double threshold,Data *data)
{

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
		k[entityPairsIterator]=(double)ktemp/data->mentionsPerEntityPairCount[entityPairsIterator];
		
	}
	

	return k;
}


double * Evaluation::getKForEntityPairs(Data *data,double threshold)
{
	double *cpeMentions= data->cpeMentions;
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
		cout<< ktemp<<"  "<<data->mentionsPerEntityPairCount[entityPairsIterator]<<endl;
		k[entityPairsIterator]=(double)ktemp/data->mentionsPerEntityPairCount[entityPairsIterator];	
	}
	return k;
}


int * Evaluation::findLabelsBasedOnMentions(double *cpeMentions,double threshold,Data *data)
{
	int *entityPairsLabel = (int * ) malloc(data->entityCount*sizeof(int));
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

int * Evaluation::findLabelsBasedOnEntity(double *cpeMentions,double threshold,Data *data)
{
	int *entityPairsLabel = (int * ) malloc(data->entityCount*sizeof(int));
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




double Evaluation::getFscore(int *predictedEntityLabels, int *entityLabels,Data *data)
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
