#ifndef UTILITY_H
#define UTILITY_H

#include "../image/image.h"
#include <opencv2/opencv.hpp>
//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
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
		static void ROI_Processing(image &src, image &tgt, std::string fileIn, std::string fileOut, std::string filetype, std::vector<ROI> ROIs, int num, int angle);


		/*Function for HW2, takes in 2 intensities A and B, and streches the images histogram to fit into them*/
		static void histoStretch(image &src, image &tgt, std::string fileIn, std::string fileOut, int A, int B, int x, int y, int sx, int sy);

        /*Function for HW2. Uses OpenCV's histogram equalization*/
        static void histoEqualization(image &src, image &tgt, int x, int y, int sx, int sy);
        /*Function for HW2, Uses OpenCV's histogram equalization and a threshold to focus on particular parts of ROI */
        static void histoEqualizationThreshold(image &src, image &tgt, int T, int x, int y, int sx, int sy);

        /*Function for HW2, OpenCV HE on the rgb color space, on channels specified by user*/
        static void histoEqualizationRGB(image &src, image &tgt, int R, int G, int B, int x, int y, int sx, int sy);

        /*Function for HW2, OpenCV on the HSV color space, on channels specified by user*/
        static void histoEqualizationHSV(image &src, image &tgt, int H, int S, int V, int x, int y, int sx, int sy);

        /*Function for HW3, high/low pass filter
         * ROI should be square and have edge size of 2^p
         * uses the radius to create the filter
         * in and out are used to detemine whether it is a high or low pass filter
         * in = 0, out = 1 is highpass
         * int = 1, out = 0 is lowpass*/

        static void high_low_pass(image &src, image &tgt, std::string fileIn, std::string fileOut, int radius, int inside, int outside, int x, int y, int sx, int sy);

        /*Function for HW3, unsharp masking
         *Takes in a radius for the lowpass filter
         * and an alpha variable that determines how much we multiply the filter by*/
        static void unsharpMasking(image &src, image &tgt, std::string fileIn, std::string fileOut, int radius, float alpha, int x, int y, int sx, int sy);


        /*Function for HW3, bandstop
         * funcctions like our high_low_pass function but we have 2 radii to create the filter */
        static void bandstop(image &src, image &tgt, std::string fileIn, std::string fileOut, int radiusA, int radiusB, int x, int y, int sx, int sy);

        /* Functions for HW3, hsv low/highpass filter
         * takes in the radius for the lowpass filter, as well as which channel (hsv) to do the operation on */
        static void high_low_pass_hsv(image &src, image &tgt, std::string fileIn, std::string fileOut, int radius, int inside, int outside, int H, int S, int V, int x, int y, int sx, int sy);

        /* Function for HW3, hsv bandstop filter
         * takes in two radii and the hsv channel to do the filter on */
        static void bandstop_hsv(image &src, image &tgt, std::string fileIn, std::string fileOut, int radiusA, int radiusB, int H, int S, int V, int x, int y, int sx, int sy);

};

#endif