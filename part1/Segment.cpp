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
	std::string output;
	output = "output.png";

  // Initialize ImageMagick install location for Windows
	Magick::InitializeMagick(*argv);

	try
	{
		Magick::Image image(fname);
		image.segment(1.0, 1.5);
		
		Magick::Geometry size;
		size = image.size();
		int height = size.height();
		int width = size.width();
		std::cout << "Image size is: " << height << "x" << width << std::endl;
	
		int segments = 0;
		int s[width][height];

		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				s[i][j] = -1;
			}
		}

		Magick::ColorYUV pixel;
		double y,u,v;
		double y1,u1,v1;
		int h_min,h_max;
		int w_min,w_max;
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				// Check all neighbors
				h_min = i - 1;
				h_max = i + 1;
				if(h_min < 0)
				{
					h_min = 0;
				}
				if(h_max > (height -1))
				{
					h_max = height - 1;
				}

				w_min = j - 1;
				w_max = j + 1;
				if(w_min < 0)
				{
					w_min = 0;
				}
				if(w_max > (width -1))
				{
					w_max = width - 1;
				}

				int found = 0;
				for(int n = h_min; n <= h_max; n++)
				{
					for(int m = w_min; m <= w_max; m++)
					{
		//				std::cout << "(" << i << "," << j << ") is now checking neighbor (" << n << "," << m << ")" << std::endl;
						if(s[n][m] > 0)
						{
		//					std::cout << "(" << n << "," << m << ") has a segment" << std::endl;
							pixel = image.pixelColor(i,j);
							y = pixel.y();
							u = pixel.u();
							v = pixel.v();
							pixel = image.pixelColor(n,m);
							y1 = pixel.y();
							u1 = pixel.u();
							v1 = pixel.v();
		//					std::cout << "comparing (" << y << "," << u << "," << v << ") with (" << y1 << "," << u1 << "," << v1 << ")" << std::endl;
							if((y == y1) && (u == u1) && (v == v1))
							{
		//						std::cout << "Found! (" << n << "," << m << ") segment is" << s[n][m] << std::endl;
								s[i][j] = s[n][m];
								found = 1;
							}
						}
					}
				}
				if(found == 0)
				{
					segments++;
					s[i][j] = segments;
		//			std::cout << "Not found, (" << i << "," << j << ") segment is " << s[i][j] << std::endl;
				}
			}
		}

		std::cout << "I found: " << segments << " segments" << std::endl;
		image.write(output);
	}
	catch(Magick::Exception &error_)
	{
		std::cout << "Whoops!  An error occured" << std::endl;
		std::cout << error_.what() << std::endl;
	}
	return 0;
}

