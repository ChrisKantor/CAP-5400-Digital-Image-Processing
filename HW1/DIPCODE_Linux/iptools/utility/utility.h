#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <sstream>
#include <math.h>
#include <vector>
#include <strings.h>
#include <string.h>

class utility
{
	public:
		utility();
		static std::string intToString(int number);
		static int checkValue(int value);

		//ROI Class
		class ROI
		{
			public:
				int x;
				int y;
				int sx;
				int sy;
				std::string op;
				std::vector<std::string> params;
				
				//our constructor
				ROI()
				{
					x = 0;
					y = 0;
					sx = 0;
					sy = 0;
					op = "";
					params.clear();
				}


				//Prints the info in the ROI
				void printROI()
				{
					std::cout << "X coord: " << x << "\n"
					<< "Y coord: " << y << "\n"
					<< "Size X: " << sx << "\n"
					<< "Size Y: " << sy << "\n"
					<< "op: " << op << "\n"
					<< "Params: \n";

					for (int i = 0; i < params.size(); i++)
					{
						std::cout << params[i] << " ";
					}

					std::cout << "\n";
				}
		};



		//Functions changed in HW1 to work with an ROI
		//we return an image to keep a track of the ROI pixels seperately
		static void addGrey(image &src, image &tgt, int value, int x, int y, int sx, int sy);
		static void binarize(image &src, image &tgt, int threshold, int x, int y, int sx, int sy);
		static void scale(image &src, image &tgt, float ratio, int x, int y, int sx, int sy);
		static void Rotate(image &src, image &tgt, ROI ROI, int angle);
		//new function for HW0, Given the threshold T and value V; if pixel intensity is below T, then decrease it by V.
		//changed in HW1 to work inside an ROI
		static void valueThreshold(image &src, image &tgt, int threshold, int value, int x, int y, int sx, int sy);

		//functions for color images
		static void addColor(image &src, image &tgt, int value, int x, int y, int sx, int sy);
		static void multColor(image &src, image &tgt, float value, int x, int y, int sx, int sy);
		static void RotateColor(image &src, image &tgt, ROI ROI, int angle);

		/*Function for HW1, Takes in all of the ROIs and prints them*/
		static void ROI_Processing(image &src, image &tgt, std::string filetype, std::vector<ROI> ROIs, int num, int angle);
};

#endif