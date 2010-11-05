#include <iostream>
#include <fstream>
#include "Sifter.h"
#include "windows.h"

void IndexDirectory(const char *, int, int);
void IndexFile(std::string, std::string, Sifter *, int, int);

int main(int argc, const char * argv[]){
	
	if(argc != 4)
	{
		std::cout << "Usage: " << std::endl;
		std::cout << "SiftExtractor.exe <k - shapes> <l - descriptors> <image directory>" << std::endl;
		return 1;
	}
	int newK = atoi(argv[1]);
	int newL = atoi(argv[2]);
	IndexDirectory(argv[3], newK, newL);
/*	Sifter test;
	std::vector<std::vector<int>> exam;
	exam = test.keynodeSetExtract(argv[1], 10,10);
	for(int i = 0; i < exam.size(); i++)
	{
		for(int j = 0; j < exam[i].size(); j++)
		{
			std::cout << exam[i][j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "DONE!" << std::endl;*/

	return 0;
}

void IndexDirectory(const char *foldername, int k, int l)
{
	WIN32_FIND_DATAA findData;
	char findPattern[MAX_PATH];
	sprintf(findPattern, "%s*", foldername);
	HANDLE hFind = FindFirstFileA(findPattern, &findData);
	Sifter siftOMatic;
	std::string fullName = "";
	std::string fn = "";
	std::ofstream outputfile;
	outputfile.open("output.txt");
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				fullName = foldername;
				fn = findData.cFileName;
				fullName += fn;
				std::cout << "Now indexing: " << fullName << std::endl;
				IndexFile(fullName, fn, &siftOMatic, k, l);
				// Index image...
			}
		}while(FindNextFileA(hFind, &findData));
	}
	outputfile.close();
}

void IndexFile(std::string filename, std::string image, Sifter *siftIt, int k, int l)
{
	std::vector<std::vector<int>> extract = siftIt->keynodeSetExtract(filename, k, l);
	std::ofstream f;
	f.open("output.txt", std::ios_base::app);
	for(int i = 0; i < extract.size(); i++)
	{
		f << image << ",";
		std::cout << image << ",";
		for(int j = 0; j < extract[i].size(); j++)
		{
			f << extract[i][j];
			std::cout << extract[i][j];
			if(j+1 != extract[i].size())
			{
				std::cout << ",";
				f << ",";
			}
		}
		f << std::endl;
		std::cout << std::endl;
	}
	f.close();
	
}