/*
 * Author: Rick Strong
 * Date: 9/13/2010
 * CSE 515 - Project: Part 1
 *
 * Purpose:
 * This program segments the image (cluster threshold = 1.0; smoothing threshold = 1.5; YUV color space ) 
 * and outputs the number of resulting segments on the top left corner of the output image
 *
 * Usage:
 * $>segment <input image name>
 */

#include <Magick++.h>
#include <string>
#include <iostream>

int main(int argc, char * argv[])
{
	std::cout << "CSE 515 Project 1 - Part 1" << std::endl;

	if(argc != 2)
	{
		std::cout << "Error parsing command line arguments." << std::endl;
		std::cout << "Usage: \nsegement <image-name>" << std::endl;
		return 1;
	}

	// Get filename
	char **filename = &argv[1];
	std::string fname(*filename);
	std::cout << "Using filename: " << fname << std::endl;

  // Initialize ImageMagick install location for Windows
	Magick::InitializeMagick(*argv);

	try
	{
		Magick::Image image(fname);
	}
	catch(Magick::Exception &error_)
	{
		std::cout << "Whoops!  An error occured" << std::endl;
		std::cout << error_.what() << std::endl;
	}
	return 0;
}
