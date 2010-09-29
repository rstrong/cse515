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
#include <queue>
#include <iostream>
#include <sstream>

struct point
{
	int x;
	int y;
};

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
		int s[height][width];
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				s[i][j] = -1;
			}
		}

		std::queue<point> toCheck;

		Magick::ColorYUV pixel;
		double y,u,v;
		double y1,u1,v1;
		int h_min,h_max;
		int w_min,w_max;
		for(int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{

				point add;
				add.x = i;
				add.y = j;
				if(s[add.x][add.y] < 0)
				{
					toCheck.push(add);
				}
				// Check all neighbors
				while(! toCheck.empty())
				{
					point next = toCheck.front();
					toCheck.pop();

					while(( !toCheck.empty()) && (s[next.x][next.y] > 0))
					{
						next = toCheck.front();
						toCheck.pop();
					}
					h_min = next.x - 1;
					h_max = next.x + 1;
					if(h_min < 0)
					{
						h_min = 0;
					}
					if(h_max > (height -1))
					{
						h_max = height - 1;
					}

					w_min = next.y - 1;
					w_max = next.y + 1;
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
							if(s[n][m] > 0)
							{
								pixel = image.pixelColor(next.x,next.y);
								y = pixel.y();
								u = pixel.u();
								v = pixel.v();
								pixel = image.pixelColor(n,m);
								y1 = pixel.y();
								u1 = pixel.u();
								v1 = pixel.v();
								// TODO output all these in an else to see wtf values they think are off
								if((y == y1) && (u == u1) && (v == v1))
								{
									s[next.x][next.y] = s[n][m];
									found = 1;
									point p;
									for(int nn = h_min; nn <= h_max; nn++)
									{
										for(int mm = w_min; mm <= w_max; mm++)
										{
											p.x = nn;
											p.y = mm;
											if(s[p.x][p.y] == -1)
											{
												s[p.x][p.y] = -2;
												toCheck.push(p);
											}
										}
									}

								}
							}
						}
					}
					if(found == 0)
					{
						segments++;
						//std::cout << "new seg#" << segments << " " << next.x << "," << next.y << std::endl;
						s[next.x][next.y] = segments;
					}
				}
			}
		}

		std::cout << "I found: " << segments << " segments" << std::endl;
			
		std::string segString;
		std::ostringstream oss;
		oss << segments << "\n";
		segString = oss.str();
		image.fillColor("white");
		image.fontPointsize(15);
		image.annotate(segString,Magick::NorthWestGravity);
		image.write(output);
		std::cout << "Saved as output.png" << std::endl;
	}
	catch(Magick::Exception &error_)
	{
		std::cout << "Whoops!  An error occured" << std::endl;
		std::cout << error_.what() << std::endl;
	}
	return 0;
}

