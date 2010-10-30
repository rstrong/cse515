
#include <iostream>
#include <fstream>
#include <string>
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
	for(int i = targetK; i > 0; i--)
	{
		std::vector<float> row;

		for(int j = 0; j < 4; j++)
		{
			row.push_back(myp[i][j]);
		}
		for(int j = 0; j < targetL; j++)
		{
			row.push_back(myk[i][j]);
		}
		pruned.push_back(row);
	}
	
	return pruned;
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
