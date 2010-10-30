#pragma once
#include "Sift.h"

class Sifter
{
public:
	Sifter(void);
	~Sifter(void);
	void runSift (std::string, Sift *);
	std::vector<std::vector<float>> keynodeSetExtract(std::string, int, int);
};

