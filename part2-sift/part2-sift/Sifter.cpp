
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
	// get our pgm file
	try
	{
		std::cout << "working on " << fname << std::endl;
		Magick::Image image(fname);
		//std::string output = "temp.pgm";
		//image.write(output);
	}
	catch(Magick::Exception &error_)
	{
		std::cout << "Error!: " << std::endl;
		std::cout << error_.what() << std::endl;
	}
	// Add imagemagick code to convert to pgm

	// Basically will contain all the runSift code

	// Now I have two vectors containing k and l; i'll prune k (and delete relevant rows of l)

	// Then you can do your magick on the l vector

	// now we just combine our k and l vectors together and return...think that works...
	// combining them just to float; inefficient but I think it works...
	std::vector<std::vector<float>> pruned;
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
	test.keynodeSetExtract("example1.png", 10,128);
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
