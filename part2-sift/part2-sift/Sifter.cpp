
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include "Sifter.h"
#include "Sift.h"
#include <Magick++.h>


Sifter::Sifter(void)
{
}

// This will replace runSift and return a combined vector of vectors...
// parameters are a string that is the filename of the original (unsegemented) image
// targetK and targetL specified
std::vector<std::vector<float>> Sifter::keynodeSetExtract(std::string fname, int targetK, int targetL)
{
	
	try
	{
		// convert image to pgm
	}
	catch(Magick::Exception &error_)
	{
		std::cout << "Error!: " << std::endl;
		std::cout << error_.what() << std::endl;
	}

	// can't get image magick working properly under windows...ignoring for now

	//for now just use temp.pgm file
	std::string pgmFile = "temp.pgm";
	Sift *sdata;
	sdata = new Sift();
	this->runSift(pgmFile,sdata);

	// so this is confusing and I probably should change it
	//get P returns 4 float K points
	// get K returns descriptors
	std::vector<std::vector<float>> myp = sdata->getP();
	std::vector<std::vector<int>> myk = sdata->getK();

	std::vector<std::vector<float>> pruned;
	if(targetK > myp.size())
	{
		targetK = myp.size();
	}
	if(targetL > 128)
	{
		targetL = 128;
	}
	
	float ratioCheck = targetK / myp.size();
	bool justTop = false;
	if(ratioCheck > 0.25)
	{
		// just return top sizes
		justTop = true;
	}

	int numFor25 = 0.25* myp.size(); // the number of our "top 25%"
	if(justTop == true)
	{
		numFor25 = targetK;
	}
	std::vector<int> candidates; // contains the indexes of our candidates for being a k, node.  These will eventually be the 
	
	// points that get clustered


	
	typedef std::map<float, int> MapType;
	MapType my_map;	
	for(int i = 0; i < myp.size(); i++)
	{
		my_map.insert(std::pair<float,int>(myp[i][2],i));
	}
	MapType::iterator iter = my_map.begin();
	int i = 0;
	for(MapType::const_iterator end = my_map.end(); end != iter; --end)
	{
		if(end->second > 0)
		{
			if(i < numFor25)
			{
				candidates.push_back(end->second);
				i++;
			}
		}
	}
	std::vector<int> final;

	if(justTop == false)
	{
		final = this->kMeans(targetK,candidates,myp);
	}
	else
	{
		final = candidates;
	}
	
	std::vector<std::vector<float>> prunedP;
	std::vector<std::vector<int>> prunedL;

	for(int i = 0; i < final.size(); i++)
	{
		prunedP.push_back(myp[final[i]]);
		prunedL.push_back(myk[final[i]]);
	}
	

	// for example, printing all descriptors
	for(int i = 0; i < prunedL.size(); i++)
	{
		for(int j = 0; j < prunedL[i].size(); j++)
		{
			std::cout << prunedL[i][j] << " ";
		}
		std::cout << std::endl;
	}

	// ACE - Prune each L here 
	// pruned L contains k vectors of descriptors; this k is our selected k


	
	// combine rows of P and L and return

	return pruned;
}

std::vector<int> Sifter::kMeans(int targetK, std::vector<int> candidates, std::vector<std::vector<float>> data)
{
	std::vector<std::vector<int>> member;
	std::vector<std::vector<float>> centroids;

	//initialize our members, just to top k
	for(int i = 0; i < targetK; i++)
	{
		std::vector<int> temp;
		temp.push_back(candidates[i]);
		member.push_back(temp);
		std::vector<float> ftemp;
		ftemp.push_back(0);
		ftemp.push_back(0);
		centroids.push_back(ftemp);
	}

	// initialize centroid vector
	

	// let's just start with 5...
	for(int i = 0; i < 5; i++)
	{
		//re compute centroid
		//for every cluster
		for(int j = 0; j < centroids.size(); j++)
		{
			std::vector<int> c = member[j]; // get members in this cluster
			float x = 0;
			float y = 0;
			for(int k = 0; k < c.size(); k++)
			{
				x += data[c[k]][0]; // add x value for each member of cluster
				y += data[c[k]][1]; // add y value for each member of cluster
			}
			x = x / c.size();
			y = y / c.size();
			centroids[j][0] = x;
			centroids[j][1] = y;
		}
		// now we clear out our memberships
		for(int j = 0; j < member.size(); j++)
		{
			member[j].clear();
		}
		// now we re-assign membership
		for(int j = 0; j < candidates.size(); j++)
		{
			int myCluster;
			myCluster = this->getClosestCentroid(centroids,data[candidates[j]][0],data[j][1]);
			member[myCluster].push_back(candidates[j]);
		}
	}

	std::vector<int> final;
	// now we find the members closest to their centroids
	for(int i = 0; i < centroids.size(); i++)
	{
		float distance = 10000;
		int winner = 0;
		for(int j = 0; j < member[i].size(); j++)
		{
			float d = this->distance(centroids[i][0],centroids[i][1],data[member[i][j]][0],data[member[i][j]][1]);
			if(d < distance)
			{
				distance = d;
				winner = member[i][j];
			}
		}
		final.push_back(winner);
	}
	
	return final;
}

int Sifter::getClosestCentroid(std::vector<std::vector<float>> cent,float x,float y)
{
	//first assume closest centroid is 0
	int closest = 0;
	float distance = this->distance(x,y,cent[0][0],cent[0][1]);

	//then see if another one is clsoer
	for(int i =1; i < cent.size(); i++)
	{
		float checkD = this->distance(x,y,cent[i][0],cent[i][1]);
		if(checkD < distance)
		{
			distance = checkD;
			closest = i;
		}
	}

	return closest;
}

float Sifter::distance(float x1, float y1, float x2, float y2)
{
	float d;
	float xd = (x1 - x2);
	xd = xd * xd;
	float xy = (y1 - y2);
	xy = xy * xy;
	d = sqrt(xd + xy);
	return d;
}

void Sifter::runSift(std::string input, Sift *sdata)
{
	std::ifstream pgmfile;
	std::ifstream siftfile;
	pgmfile.open(input);
	if(pgmfile.is_open())
	{
		pgmfile.close();
		
	}
	else
	{
		std::cout << "Could not open pgm file: " << input << std::endl;
	}

	std::string c = "sift.exe " + input;
	const char *command = c.c_str();
	std::cout << "running: " << command << std::endl;
	std::system(command);
	std::string siftname = input;
	size_t pos;
	pos = siftname.find(".");
	siftname = siftname.substr(0,pos);
	siftname = siftname + ".sift";
	
	siftfile.open(siftname);
	if(siftfile.is_open())
	{
		std::string line;
		while(siftfile.good())
		{
			std::getline(siftfile,line);
			sdata->addLine(line);
		}

	}
	else
	{
		std::cout << "Error opening sift file!" << std::endl;
	}


}


Sifter::~Sifter(void)
{
}

int main(int argc, const char * argv[]){
	Sifter test;
	std::vector<std::vector<float>> exam;
	exam = test.keynodeSetExtract("example1.png", 10,10);
	for(int i = 0; i < exam.size(); i++)
	{
		for(int j = 0; j < exam[i].size(); j++)
		{
			std::cout << exam[i][j] << " ";
		}
		std::cout << std::endl;
	}
	/*Sift *data;
	data = new Sift();

	char *f = "example2.pgm";
	test.runSift(f,data);

	std::cout << "Let's test the data..." << std::endl;
	std::vector<std::vector<float>> myp = data->getP();
	std::vector<std::vector<int>> myk = data->getK();
	std::cout << "p size: " << myp.size() << std::endl;
	std::cout << "k size: " << myk.size() << std::endl;
	int i = 0;
	std::cout << "first p: " << std::endl;
	for(i = 0; i < 4; i++)
	{
		std::cout << myp[0][i] << ", " << std::endl;
	}*/
	/*std::cout << "first k:" << std::endl;
	for(i = 0; i < 128; i++)
	{
		std::cout << myk[0][i] << ", " << std::endl;
	}*/
	return 0;
}
