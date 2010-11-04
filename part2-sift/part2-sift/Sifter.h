#pragma once
#include "Sift.h"

class Sifter
{
public:
	Sifter(void);
	~Sifter(void);
	void runSift (std::string, Sift *);
	std::vector<std::vector<float>> keynodeSetExtract(std::string, int, int);
	void Sifter::getSortedIndices(std::vector<std::vector<float>>, std::vector<int>, int, int);
	std::vector<int> Sifter::kMeans(int, std::vector<int>, std::vector<std::vector<float>>);
	int Sifter::getClosestCentroid(std::vector<std::vector<float>>,float,float);
	float Sifter::distance(float, float, float, float);
	std::vector<int> Sifter::dPruneByJoin(std::vector<int>, int, int);
	std::vector<int> Sifter::dPruneByDrop(std::vector<int>, int, int);
};

