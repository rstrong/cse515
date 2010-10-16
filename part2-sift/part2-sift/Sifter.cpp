
#include <iostream>
#include <fstream>
#include <string>
#include "Sifter.h"
#include "Sift.h"

Sifter::Sifter(void)
{
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
	Sift *data;
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
	}
	std::cout << "first k:" << std::endl;
	for(i = 0; i < 128; i++)
	{
		std::cout << myk[0][i] << ", " << std::endl;
	}
	return 0;
}
