#include"evaluations.h"
#include <algorithm>
#include<fstream>
#include<string.h>
#include <sstream>

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

double Evaluation::findBestMacroThreshold(const double *cpeEntityPairs,const Data *data,int relationNumber,double *fScore,const Data *validationData,const double *cpeValidMentions)
{
	double *thresholdValues= (double * ) malloc((data->entityCount + 1)*sizeof(double)); //+1 coz to include both borders

	double *sortedCpeEntityPairs= sortArray(cpeEntityPairs,data->entityCount);

	int entityPairsIterator=0;
	/*
	thresholdValues[entityPairsIterator]=sortedCpeEntityPairs[0]-0.01;
	if(thresholdValues[entityPairsIterator]<0)
		thresholdValues[entityPairsIterator]=0.0001;
	for(entityPairsIterator=1; entityPairsIterator<data->entityCount ;  entityPairsIterator++)
	{
		thresholdValues[entityPairsIterator]= (sortedCpeEntityPairs[entityPairsIterator-1]+sortedCpeEntityPairs[entityPairsIterator])/2;
	}
	thresholdValues[entityPairsIterator]= sortedCpeEntityPairs[entityPairsIterator -1]+0.01;
	*/

	for(entityPairsIterator=0; entityPairsIterator<data->entityCount ;  entityPairsIterator++)
	{
		thresholdValues[entityPairsIterator]= sortedCpeEntityPairs[entityPairsIterator];
	}
	thresholdValues[entityPairsIterator]= sortedCpeEntityPairs[entityPairsIterator-1];
	double maxFscore=0;
	double bestThreshold=0;

	for(int iterator=0;iterator<data->entityCount+1;iterator++)
	{
		// double *predictedEntityPairsLabel = findLabelsBasedOnEntity(cpeEntityPairs,thresholdValues[iterator],data->entityCount);
		// double tempFscore=getFscore(predictedEntityPairsLabel,data->allLabels[relationNumber],data->entityCount,1);
		double *predictedEntityPairsLabel = findLabelsBasedOnMentions(cpeValidMentions,thresholdValues[iterator],validationData);
		double tempFscore = getFscore(predictedEntityPairsLabel,validationData->allLabels[relationNumber],validationData->entityCount,1);/*beta*/
		if(tempFscore>maxFscore)
		{
			maxFscore=tempFscore;

			bestThreshold= thresholdValues[iterator];
		}
		free(predictedEntityPairsLabel);
	}

	free(thresholdValues);
	free(sortedCpeEntityPairs);
	// cout<<"Best training Fscore on training data "<<maxFscore<<"\t relationNumber : "<<relationNumber<<endl;
	*fScore=maxFscore;
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


double Evaluation::getAccuracy(const double *predictedEntityLabels,const double *entityLabels,int entityCount)
{
	int truePredictives=0;
	for(int i=0;i<entityCount;i++)
	{
		if(predictedEntityLabels[i]==entityLabels[i])
			truePredictives++;
	}
	return (double)truePredictives/(double)entityCount;
}


double Evaluation::getFscore(const double *predictedEntityLabels,const double *entityLabels, int entityCount,double beta)
{
	double precision;
	double recall;
	
	
	//cout<<"get F scor fundtion"<<endl;
	int TP=0;
	int FP=0;
	int FN=0;
	int TN=0;
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
		}else{
			TN++;
		}
	}
	double fvalue=0;
	
	if(TP==0)
		return fvalue;
	precision= (double)TP/(TP+FP);
	recall= (double)TP/(TP+FN);
	

	if(beta==-1)
	{	
		fvalue=-1;
		double tempF=0;
		for(beta=1;beta<3;beta=beta+0.5)
		{
			double betaSquare=beta*beta;

			if(precision>0 && recall>0) 
				tempF=(1+(betaSquare))*(precision*recall)/((betaSquare)*precision+recall);
			if(tempF>fvalue)
				fvalue=tempF;
		}
	}else
	{
		double betaSquare=beta*beta;
		if(precision>0 && recall>0) 
		fvalue=(1+(betaSquare))*(precision*recall)/((betaSquare)*precision+recall);

	}	
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


std::string to_string2(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

void myPrintToFileDouble(double *a,double *b,double thresh,int size,int re)
{
	std::ofstream ofs;
  	
  	//string path = "temp/"+fname+"_"+to_string(relationNumber)+".txt";
  	string path="temp/predictedLabelsAndTrueLabel_Rel_"+to_string2(re);
  	ofs.open (path.c_str(), std::ofstream::out );
	for (int i=0;i<size;i++)
	{
		
		// ofs<<i<<"\t"<<a[i]<<"\t"<<s<<"\t"<<thresh<<endl;
		if(i!=0)
			ofs<<endl;
		ofs<<a[i]<<"\t"<<b[i];
	}
	ofs.close();
}


double Evaluation::getFScore(const Data *data,double threshold,const double *cpeMentions,int relationNumber,double beta)
{
	//double *entityCpeMentions = getMaxCpePerEntityPair(data,cpeMentions);	
	//double *predictedEntityLabels = findLabelsBasedOnEntity(entityCpeMentions,threshold,data);
	double *predictedEntityPairsLabel = findLabelsBasedOnMentions(cpeMentions,threshold,data);
	double tempFscore=getFscore(predictedEntityPairsLabel,data->allLabels[relationNumber],data->entityCount,beta); ///
	//myPrintToFileDouble(data->allLabels[relationNumber],entityCpeMentions,threshold,data->entityCount,relationNumber);
	double accuracy= getAccuracy(predictedEntityPairsLabel,data->allLabels[relationNumber],data->entityCount);
	myPrintToFileDouble(data->allLabels[relationNumber],predictedEntityPairsLabel,threshold,data->entityCount,relationNumber);
	//free(entityCpeMentions);
	
	cout<<"---> relationNumber "<<relationNumber<<"\t Accuracy "<<accuracy<<endl;
	free(predictedEntityPairsLabel);
	return tempFscore;
	///return getFScore(predictedEntityLabels,data->trueEntityLabels,data);

}