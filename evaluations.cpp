#include"evaluations.h"
#include <algorithm>

using namespace std;

Evaluation::Evaluation()
{

}


double *  Evaluation::getMaxCpePerEntityPair(const Data *data,const double *cpeMentions)
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
		double *predictedEntityPairsLabel = findLabelsBasedOnEntity(cpeEntityPairs,thresholdValues[iterator],data->entityCount);
		double tempFscore=getFscore(predictedEntityPairsLabel,data->allLabels[relationNumber],data->entityCount);
		if(tempFscore>maxFscore)
		{
			maxFscore=tempFscore;
			bestThreshold= thresholdValues[iterator];
		}
		free(predictedEntityPairsLabel);
	}

	free(thresholdValues);
	free(sortedCpeEntityPairs);
	cout<<"Best training Fscore on training data "<<maxFscore<<"\t relationNumber : "<<relationNumber<<endl;
	return bestThreshold;
}


double * Evaluation::sortArray(const double *cpeEntityPairs,int SIZE)
{
	double *sortedCpeEntityPairs=(double * ) malloc((SIZE)*sizeof(double));
	std::copy(cpeEntityPairs,cpeEntityPairs + SIZE, sortedCpeEntityPairs);
	sort(sortedCpeEntityPairs, sortedCpeEntityPairs + SIZE);
	return sortedCpeEntityPairs;
}

double * Evaluation::findLabelsBasedOnEntity(const double *cpeMentions,double threshold,int entityCount)
{
	double *entityPairsLabel = (double * ) malloc(entityCount*sizeof(double));
	int mentionsIterator=0;
	for(int entityPairsIterator=0; entityPairsIterator<entityCount ;  entityPairsIterator++)
	{
	
		if(cpeMentions[entityPairsIterator]>threshold)
			entityPairsLabel[entityPairsIterator]=1;
		else
			entityPairsLabel[entityPairsIterator]=0;
	}
	return entityPairsLabel;
}


double Evaluation::getFscore(const double *predictedEntityLabels,const double *entityLabels, int entityCount)
{
	double precision;
	double recall;
	double beta=0.5;
	double betaSquare=beta*beta;

	//cout<<"get F scor fundtion"<<endl;
	int TP=0;
	int FP=0;
	int FN=0;
	for(int entityPairsIterator=0; entityPairsIterator<entityCount ;  entityPairsIterator++)
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

	return fvalue;

}


double * Evaluation::getKForEntityPairs(const Data *data,double threshold,double *cpeMentions,int relationNumber)
{
	//double *cpeMentions= data->cpeMentions;
	double *k = (double * ) malloc(data->entityCount*sizeof(double));
	int mentionsIterator=0;
	double *lables = data->allLabels[relationNumber];
	for(int entityPairsIterator=0; entityPairsIterator<data->entityCount ;  entityPairsIterator++)
	{
		int iterator=0;
		int ktemp=0;

		if(lables[entityPairsIterator]==0)
		{
			k[entityPairsIterator]=0;
			continue;
		}

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


double Evaluation::getFScore(const Data *data,double threshold,double *cpeMentions,int relationNumber)
{
	double *entityCpeMentions = getMaxCpePerEntityPair(data,cpeMentions);	
	//double *predictedEntityLabels = findLabelsBasedOnEntity(entityCpeMentions,threshold,data);
	double *predictedEntityPairsLabel = findLabelsBasedOnEntity(entityCpeMentions,threshold,data->entityCount);
	double tempFscore=getFscore(predictedEntityPairsLabel,data->allLabels[relationNumber],data->entityCount); ///
	free(entityCpeMentions);
	free(predictedEntityPairsLabel);
	return tempFscore;
	///return getFScore(predictedEntityLabels,data->trueEntityLabels,data);

}