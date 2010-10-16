#pragma once
#include <string>
#include <vector>

class Sift
{
	std::vector<std::vector<float>> p;
	std::vector<std::vector<int>> k;
public:
	Sift(void);
	~Sift(void);
	void addLine(std::string);
	std::vector<std::vector<float>> getP();
	std::vector<std::vector<int>> getK();
};

