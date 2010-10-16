#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "Sift.h"


Sift::Sift(void)
{
}


Sift::~Sift(void)
{
}

std::vector<std::vector<float>> Sift::getP()
{
	return p;
}
std::vector<std::vector<int>> Sift::getK()
{
	return k;
}
void Sift::addLine(std::string line)
{

	std::istringstream iss(line);
	int c = 0;
	std::vector <float> newp;
	std::vector <int> newk;
	do
	{
		std::string piece;
		iss >> piece;
		
		if(c < 4)
		{
			float f = ::atof(piece.c_str());
			newp.push_back(f);
		}
		else if(c < 132)
		{
			int i = ::atoi(piece.c_str());
			newk.push_back(i);
		}
		
		c++;
		//std::cout << "substring: " << piece << std::endl;
	}while(iss);
	
	if(newp.size() == 4)
	{
		p.push_back(newp);
	}
	if(newk.size() == 128)
	{
		k.push_back(newk);
	}
	
}
