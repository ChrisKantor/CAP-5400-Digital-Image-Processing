#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}



/*-----------------------------------------------------------------------**/
/* Changed to work within an ROI in HW1 */
void utility::addGrey(image &src, image &tgt, int value, int x, int y, int sx, int sy)
{
	for (int i = x; i <= x + sx; i++)
	{
		for (int j = y; j <= y + sy; j++)
		{
			tgt.setPixel(i, j, checkValue(tgt.getPixel(i,j) + value));
		}
	}
}

/*-----------------------------------------------------------------------**/
/* Changed to work within an ROI in HW1. This one works on color images */
void utility::addColor(image &src, image &tgt, int value, int x, int y, int sx, int sy)
{
	//check that value is between -100 and 100
	if (value > 100 || value < -100)
	{
		std::cout << "Value out of range";
		exit(1);
	}

	//iterate through the color channels (R = 0, G = 1, B = 2)
	for (int c = 0; c < 3; c++)
	{
		for (int i = x; i <= x + sx; i++)
		{
			for (int j = y; j <= y + sy; j++)
			{
				//make sure we get and set values to a particular channel
				tgt.setPixel(i, j, c, checkValue(tgt.getPixel(i,j, c) + value));
			}
		}
	}
}



/*-----------------------------------------------------------------------**/
/* Changed to work within an ROI in HW1. This one works on color images */
void utility::multColor(image &src, image &tgt, float value, int x, int y, int sx, int sy)
{
	//check that value is between 1 and 2
	if (value > 2 || value < 1)
	{
		std::cout << "Value out of range";
		exit(1);
	}

	//iterate through the color channels (R = 0, G = 1, B = 2)
	for (int c = 0; c < 3; c++)
	{
		for (int i = x; i <= x + sx; i++)
		{
			for (int j = y; j <= y + sy; j++)
			{
				//make sure we get and set values to a particular channel
				tgt.setPixel(i, j, c, checkValue(tgt.getPixel(i,j, c) * value));
			}
		}
	}
}



/*-----------------------------------------------------------------------**/
/* Changed to work within an ROI in HW1 */
void utility::binarize(image &src, image &tgt, int threshold, int x, int y, int sx, int sy)
{
	for (int i = x; i <= x + sx; i++)
	{
		for (int j = y; j <= y + sy; j++)
		{
			if (tgt.getPixel(i,j) < threshold)
			{
				tgt.setPixel(i,j,MINRGB);
			}

			else
			{
				tgt.setPixel(i,j,MAXRGB);
			}
		}
	}
}



/*-----------------------------------------------------------------------**/
/** in HW1, just call the x2 scale case if scale factor >= 1.5.
 * then just grab the original ROI # of pixels from it, no interpolation needed*/
void utility::scale(image &src, image &tgt, float ratio, int x, int y, int sx, int sy)
{
	//round the ratio to 1 or two depending on which its closer to
	if (ratio >= 1 && ratio < 1.5)
	{
		ratio = 1;
		return;
	}

	//round the ratio to 1 or two depending on which its closer to
	if (ratio >= 1.5 && ratio <= 2)
	{
		ratio = 2;
	}

	//create our temp array and fill it
	std::vector<std::vector<int> > temp(sx, std::vector<int>(sx,0));
	for (int i = 0; i < sx; i++)
	{
		for (int j = 0; j < sx; j++)
		{
			temp[i][j] = tgt.getPixel(x + i, y + j);
		}
	}

	for (int i = 0; i <= sx; i++)
	{
		for (int j = 0; j <= sy; j++)
		{
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			tgt.setPixel(i + x, j + y , checkValue(temp[i2][j2]));
		}
	}
}


/*-----------------------------------------------------------------------**/
/*Function for HW0, Given the threshold T and value V; if pixel intensity is below T, then decrease it by V.*/
/* Changed in HW1 to work with an ROI*/
void utility::valueThreshold(image &src, image &tgt, int threshold, int value, int x, int y, int sx, int sy)
{
	for (int i = x; i <= x + sx; i++)
	{
		for (int j = y; j <= y + sy; j++)
		{
			//if this pixel is less than the threshold, reduce it by V, then make sure it's not negative
			//if it is negative, set the new pixel to 0 instead
			if (tgt.getPixel(i,j) < threshold)
			{
				tgt.setPixel(i,j,checkValue(tgt.getPixel(i,j)-value));
			}

			//if the pixel is not less than the threshold, keep the same value
			else
			{
				tgt.setPixel(i, j, tgt.getPixel(i,j));
			}
		}
	}
}



/*-----------------------------------------------------------------------**/
/*Function for HW1, Rotates pixels that are in an ROI. Works on color images*/
void utility::RotateColor(image &src, image &tgt, ROI ROI, int angle)
{
	src.copyImage(tgt);

	//if angle is 0 or 360, we don't do anything
	if (angle == 0 || angle == 360)
	{
		return;
	}

	//We will just call this case multiple times to do any rotation
	// To rotate 90,
	// The new value of x will be: j
	// New value of y will be: original value of size of array - 1 - i
	else if (angle == 90 || angle == 180 || angle == 270)
	{
		for (int c = 0; c < 3; c++)
		{
			//create our temp array and fill it
			std::vector<std::vector<int> > temp(ROI.sx, std::vector<int>(ROI.sx,0));
			for (int i = 0; i < ROI.sx; i++)
			{
				for (int j = 0; j < ROI.sx; j++)
				{
					//temp[i][j] = arr[ROI.x + i][ROI.y + j];
					temp[i][j] = tgt.getPixel(ROI.x + i, ROI.y + j, c);
				}
			}

			//inplace rotation
			int t = 0;
			
			//find transpose using vector
			for (int i = 0; i < ROI.sx; i++)
			{
				for (int j = 0; j <= i; j++)
				{
					t = temp[i][j];
					temp[i][j] = temp[j][i];
					temp[j][i] = t;
				}
			}
			
			//reverse each row
			//using temp array
			for (int i = 0; i < ROI.sx; i++)
			{
				int start = 0;
				int end = ROI.sx - 1;

				while (start < end)
				{
					int t = temp[i][start];
					temp[i][start] = temp[i][end];
					temp[i][end] = t;

					tgt.setPixel(i + ROI.x, start + ROI.y, c, temp[i][start]);
					tgt.setPixel(i + ROI.x, end + ROI.y, c, temp[i][end]);
					start++;
					end--;
				}
			}
		}

		RotateColor(src, tgt, ROI, angle - 90);
	}
}



/*-----------------------------------------------------------------------**/
/*Function for HW1, Rotates pixels that are in an ROI*/
void utility::Rotate(image &src, image &tgt, ROI ROI, int angle)
{
	src.copyImage(tgt);

	//if angle is 0 or 360, we don't do anything
	if (angle == 0 || angle == 360)
	{
		return;
	}

	//We will just call this case multiple times to do any rotation
	// To rotate 90,
	// The new value of x will be: j
	// New value of y will be: original value of size of array - 1 - i
	else if (angle == 90 || angle == 180 || angle == 270)
	{
		//create our temp array and fill it
		std::vector<std::vector<int> > temp(ROI.sx, std::vector<int>(ROI.sx,0));
		for (int i = 0; i < ROI.sx; i++)
		{
			for (int j = 0; j < ROI.sx; j++)
			{
				//temp[i][j] = arr[ROI.x + i][ROI.y + j];
				temp[i][j] = tgt.getPixel(ROI.x + i, ROI.y + j);
			}
		}

		//inplace rotation
		int t = 0;
		
		//find transpose using vector
		for (int i = 0; i < ROI.sx; i++)
		{
			for (int j = 0; j <= i; j++)
			{
				t = temp[i][j];
				temp[i][j] = temp[j][i];
				temp[j][i] = t;
			}
		}
		
		//reverse each row
		//using temp array
		for (int i = 0; i < ROI.sx; i++)
		{
			int start = 0;
			int end = ROI.sx - 1;

			while (start < end)
			{
				int t = temp[i][start];
				temp[i][start] = temp[i][end];
				temp[i][end] = t;

				tgt.setPixel(i + ROI.x, start + ROI.y, temp[i][start]);
				tgt.setPixel(i + ROI.x, end + ROI.y, temp[i][end]);
				start++;
				end--;
			}
		}

		Rotate(src, tgt, ROI, angle - 90);
	}
}


















/*FUNCTIONS FOR HW2*/
/*Histogram image function.
 *Creates an image of a histogram and saves it. used in histoStretch*/
void histoToImage(std::vector<int> histo, std::string filename)
{
    //i goes top to bottom, j goes left to right in (i,j) format,
    //so we will change this to get a correct looking histogram
    image img(256, 256);

    //find the most common intensity in the function
    int mostCommonIndex = 0;
    for (int i = 0; i < 256; i++)
    {
        if (histo[i] > histo[mostCommonIndex])
        {
            mostCommonIndex = i;
        }
    }

    //for every row in the output histo image
    for (int i = 0; i < 256; i++)
    {
        //if we don't have any pixels a this intensity, skip
        if (histo[i] == 0)
        {
            continue;
        }

        //normalize the value by dividing by the number of the most common intensity, then multiplying by 256
        //this essentially means that the most common intensity has a height of 256 in the histogram image
        //and all other intensities have a height proportional to the cost common intensity
        float temp = (static_cast< float >(histo[i]) / static_cast< float >(histo[mostCommonIndex])) * 256;

        //round this float value to the nearest int to get the number of pixels
        int pixels = round(temp);

        for (int j = 255; j > 256 - pixels; j--)
        {
            img.setPixel(j, i, 255);
        }
    }

    img.save(filename.c_str());
}



/*Histogram stretching. Takes in values A and B, which serve as the new range of intensities for the image*/
void utility::histoStretch(image &src, image &tgt, std::string fileIn, std::string fileOut, int A, int B, int x, int y, int sx, int sy)
{
	//create vector to serve as histogram. size 256 with all elements init to 0
	std::vector<int> histo(256, 0);

	//create histogram from image
	for (int i = x; i <= x + sx; i++)
	{
		for (int j = y; j <= y + sy; j++)
		{
			histo[tgt.getPixel(i,j)]++;
		}
	}

    //save image of histogram
    std::string beforeImg = fileOut + "_ROI_" + std::to_string(x) + "_" + std::to_string(y) + "_To_" + std::to_string(sx + x) + "_" + std::to_string(sy + y) + "_Before.pgm";
    histoToImage(histo, beforeImg);


	//find 0.95 percentile min and max from histogram
	//C and D hold the intensity value of the 5th and 95th percentile of the histogram
	int C = -1;
	int D = -1;

	//minP and maxP hold the pixel # that represents the 5th and 95th percentile
	//the more pixels in the ROI, the higher minP and maxP will be
	int minP = round(0.05 * (sx * sy));
	int maxP = round(0.95 * (sx * sy));

	//We are essentially finding the pixel corresponding to minP/maxP in the histogram
	//if minP = 200, we need to find the 200th pixel in the histogram.
	//we can do this by iterating through the histogram, and subracting the number of pixels at that intensity
	//from minP until we hit 0 or a negative number, when we do, we know that minP is at this intensity value
	for (int i = 0; i < 256; i++)
	{
		if (C == -1)
		{
			if (histo[i] < minP)
			{
				minP = minP - histo[i];
			}

			else
			{
				C = i;
			}
		}

		if (D == -1)
		{
			if (histo[i] < maxP)
			{
				maxP = maxP - histo[i];
			}

			else
			{
				D = i;
			}
		}
	}

	//stretch the histogram to fit the new range given by A and B
	//[0, C] -> A
	//[C, D] -> [A, B]
	//[D, 255] -> B
	//formula is P' = ((P - C) * ((B - A) / (D - C))) + A, where P is the current pixels intensity
	for (int i = x; i <= x + sx; i++)
	{
		for (int j = y; j <= y + sy; j++)
		{
			tgt.setPixel(i, j, checkValue((((tgt.getPixel(i,j) - C) * ((B - A) / (D - C))) + A)));
		}
	}

    //create vector to serve as histogram after stretching. size 256 with all elements init to 0
    std::vector<int> histoAfter(256, 0);

    //int min/max used to determine the min/max pixel intensity in the image while reading in for debugging
    //create histogram from image
    for (int i = x; i <= x + sx; i++)
    {
        for (int j = y; j <= y + sy; j++)
        {
            histoAfter[tgt.getPixel(i,j)]++;
        }
    }

    //save image of histogram
    std::string AfterImg = fileOut + "_ROI_" + std::to_string(x) + "_" + std::to_string(y) + "_To_" + std::to_string(sx + x) + "_" + std::to_string(sy + y) + "_After.pgm";
    histoToImage(histoAfter, AfterImg);
}



/*This function uses openCV to perform histogram equalization on an ROI*/
void utility::histoEqualization(image &src, image &tgt, int x, int y, int sx, int sy)
{
    //create cv::Mat object to hold the image
    cv::Mat in = cv::Mat(sx, sx, CV_8UC1);

    //loop through ROI, and set each pixel in the mat to the value, filling it
    for (int i = x; i < x + sx; i++)
    {
        for (int j = y; j < y + sy; j++)
        {
            in.at<uchar>(j-y, i-x) = tgt.getPixel(i, j);
        }
    }

    //perform histogram equalization
    cv::Mat out;
    cv::equalizeHist(in, out);

    //copy values from out into the tgt image
    for (int i = x; i < x + sx; i++)
    {
        for (int j = y; j < y + sy; j++)
        {
            tgt.setPixel(i, j, (int)out.at<uchar>(j-y,i-x));
        }
    }
}



/*This function uses openCV to perform histogram equalization on an ROI
 *We pass in a threshold to use for HE in this function
 *We set all pixels brighter than T to T, perform HE on that image,
 *then replace the bright pixels when we apply changes to tgt*/
void utility::histoEqualizationThreshold(image &src, image &tgt, int T, int x, int y, int sx, int sy)
{
    //create cv::Mat object to hold the image
    cv::Mat in = cv::Mat(sx, sx, CV_8UC1);

    //loop through ROI, and set each pixel in the mat to the value, filling it
    //if the src pixel is greater than T, set it to T
    for (int i = x; i < x + sx; i++)
    {
        for (int j = y; j < y + sy; j++)
        {
            if (src.getPixel(i,j) > T)
            {
                in.at<uchar>(j-y, i-x) = T;
            }

            else
            {
                in.at<uchar>(j-y, i-x) = src.getPixel(i, j);
            }
        }
    }

    //perform histogram equalization
    cv::Mat out;
    cv::equalizeHist(in, out);

    //we need to go through src and out.
    //if the pixel is greater than T in src, copy that over, as we did not do HE on it
    //if the pixel is <= T in src, we did do HE on it, so copy it from out
    for (int i = x; i < x + sx; i++)
    {
        for (int j = y; j < y + sy; j++)
        {
            if (src.getPixel(i,j) > T)
            {
                tgt.setPixel(i, j, src.getPixel(i,j));
            }

            else
            {
                tgt.setPixel(i, j, (int)out.at<uchar>(j-y,i-x));
            }
        }
    }
}



/*Performs histogram equalization using openCV on specific color channels */
void utility::histoEqualizationRGB(image &src, image &tgt, int R, int G, int B, int x, int y, int sx, int sy)
{
    if (R == 1)
    {
        //create cv::Mat object to hold the image
        cv::Mat in = cv::Mat(sx, sx, CV_8UC1);

        //loop through ROI, and set each pixel in the mat to the value, filling it
        //use the specific channel (R = 0, G = 1, B = 2)
        for (int i = x; i < x + sx; i++)
        {
            for (int j = y; j < y + sy; j++)
            {
                in.at<uchar>(j-y, i-x) = tgt.getPixel(i, j, 0);
            }
        }

        //perform histogram equalization
        cv::Mat out;
        cv::equalizeHist(in, out);

        //copy values from out into the tgt image
        for (int i = x; i < x + sx; i++)
        {
            for (int j = y; j < y + sy; j++)
            {
                tgt.setPixel(i, j, 0, (int)out.at<uchar>(j-y,i-x));
            }
        }
    }

    if (G == 1)
    {
        //create cv::Mat object to hold the image
        cv::Mat in = cv::Mat(sx, sx, CV_8UC1);

        //loop through ROI, and set each pixel in the mat to the value, filling it
        //use the specific channel (R = 0, G = 1, B = 2)
        for (int i = x; i < x + sx; i++)
        {
            for (int j = y; j < y + sy; j++)
            {
                in.at<uchar>(j-y, i-x) = tgt.getPixel(i, j, 1);
            }
        }

        //perform histogram equalization
        cv::Mat out;
        cv::equalizeHist(in, out);

        //copy values from out into the tgt image
        for (int i = x; i < x + sx; i++)
        {
            for (int j = y; j < y + sy; j++)
            {
                tgt.setPixel(i, j, 1, (int)out.at<uchar>(j-y,i-x));
            }
        }
    }

    if (B == 1)
    {
        //create cv::Mat object to hold the image
        cv::Mat in = cv::Mat(sx, sx, CV_8UC1);

        //loop through ROI, and set each pixel in the mat to the value, filling it
        //use the specific channel (R = 0, G = 1, B = 2)
        for (int i = x; i < x + sx; i++)
        {
            for (int j = y; j < y + sy; j++)
            {
                in.at<uchar>(j-y, i-x) = tgt.getPixel(i, j, 2);
            }
        }

        //perform histogram equalization
        cv::Mat out;
        cv::equalizeHist(in, out);

        //copy values from out into the tgt image
        for (int i = x; i < x + sx; i++)
        {
            for (int j = y; j < y + sy; j++)
            {
                tgt.setPixel(i, j, 2, (int)out.at<uchar>(j-y,i-x));
            }
        }
    }
}



/*Performs histogram equalization using openCV on specific color channels */
void utility::histoEqualizationHSV(image &src, image &tgt, int H, int S, int V, int x, int y, int sx, int sy)
{
    //read in ROI using 3 channels
    //create cv::Mat object to hold the image
    cv::Mat in = cv::Mat(sx, sx, CV_8UC3);
    for (int i = x; i < x + sx; i++)
    {
        for (int j = y; j < y + sy; j++)
        {
            //create an intensity vector and set each index to the value in the RGB channel
            //OpenCV uses BGR so we need to set them in reverse
            cv::Vec3b intensity;
            intensity[0] = tgt.getPixel(i, j, 2);
            intensity[1] = tgt.getPixel(i, j, 1);
            intensity[2] = tgt.getPixel(i, j, 0);
            in.at<cv::Vec3b>(j-y, i-x) = intensity;
        }
    }

    //convert in to HSV
    cv::Mat hsv;
    cv::cvtColor(in, hsv, cv::COLOR_BGR2HSV);

    //split out into a vector called channels representing H S and V channels to do HE on
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);

    //now we have an HSI image split in 3 channels
    //now we perform Histogram equalization on the channels provided by user

    if (H == 1)
    {
        cv::equalizeHist(channels[0], channels[0]);
    }

    if (S == 1)
    {
        cv::equalizeHist(channels[1], channels[1]);
    }

    if (V == 1)
    {
        cv::equalizeHist(channels[2], channels[2]);
    }

    //merge channels back together to get complete image. save it in equalized
    cv::Mat equalized;
    cv::merge(channels, equalized);

    //convert equalized back to rgb for display, saved in out
    cv::Mat out;
    cv::cvtColor(equalized, out, cv::COLOR_HSV2BGR);

    //split out to get the individual channels for access
    channels.clear();
    cv::split(out, channels);

    //need to go B G R since openCV does it in reverse
    for (int i = x; i < x + sx; i++)
    {
        for (int j = y; j < y + sy; j++)
        {
            tgt.setPixel(i, j, 0, (int)channels[2].at<uchar>(j-y,i-x));
            tgt.setPixel(i, j, 1, (int)channels[1].at<uchar>(j-y,i-x));
            tgt.setPixel(i, j, 2, (int)channels[0].at<uchar>(j-y,i-x));
        }
    }
}



















/*-----------------------------------------------------------------------------------------------**/
/*-----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------*/
/*Function for HW1, Takes in all of the ROIs, then calls the operations for each of them using the info in the ROI*/
void utility::ROI_Processing(image &src, image &tgt, std::string fileIn, std::string fileOut, std::string filetype, std::vector<ROI> ROIs, int num, int angle)
{
	//for color images (.ppm format)
	if (filetype == ".ppm")
	{
		//call each ROI
		for (int i = 0; i < num; i++)
		{
			
			//first check what operation we are doing
			//.c_str converts from a std::string to a const char*, which is what strcasecmp uses
			if (strcasecmp(ROIs[i].op.c_str(), "addColor") == 0)
			{
				addColor(src, tgt, atoi(ROIs[i].params[0].c_str()), ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
			}

			else if (strcasecmp(ROIs[i].op.c_str(), "multColor") == 0)
			{
				multColor(src, tgt, atof(ROIs[i].params[0].c_str()), ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
			}

			else if (strcasecmp(ROIs[i].op.c_str(), "binarize") == 0)
			{
				binarize(src, tgt, atoi(ROIs[i].params[0].c_str()), ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
			}

			else if (strcasecmp(ROIs[i].op.c_str(), "valueThreshold") == 0)
			{
				valueThreshold(src, tgt, atoi(ROIs[i].params[0].c_str()), atoi(ROIs[i].params[1].c_str()), ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
			}

			else if (strcasecmp(ROIs[i].op.c_str(), "scale") == 0)
			{
				//check scale factor, if its between 1 or 2
				float scaleFactor = atof(ROIs[i].params[0].c_str());

				if (scaleFactor > 2 && scaleFactor < 1)
				{
					std::cout << "Scale factor not between 1 or 2";
					exit(1);
				}

				scale(src, tgt, scaleFactor, ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
			}

            //does histogram equalization on color channels specified by user
			else if (strcasecmp(ROIs[i].op.c_str(), "histoEqualRGB") == 0)
			{
                int r = 0;
                int g = 0;
                int b = 0;

                //go through each of the params passed, if we have an r, b, or g, set it to 1
                if (ROIs[i].params[0].find("r") != std::string::npos || ROIs[i].params[0].find("R") != std::string::npos)
                {
                    r = 1;
                }
                if (ROIs[i].params[0].find("g") != std::string::npos || ROIs[i].params[0].find("G") != std::string::npos)
                {
                    g = 1;
                }
                if (ROIs[i].params[0].find("b") != std::string::npos || ROIs[i].params[0].find("B") != std::string::npos)
                {
                    b = 1;
                }

                histoEqualizationRGB(src, tgt, r, g, b, ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
			}

            //does histogram equalization on HSV channels specified by user
            else if (strcasecmp(ROIs[i].op.c_str(), "histoEqualHSV") == 0)
            {
                int h = 0;
                int s = 0;
                int v = 0;

                //go through each of the params passed, if we have an r, b, or g, set it to 1
                if (ROIs[i].params[0].find("h") != std::string::npos || ROIs[i].params[0].find("H") != std::string::npos)
                {
                    h = 1;
                }
                if (ROIs[i].params[0].find("s") != std::string::npos || ROIs[i].params[0].find("S") != std::string::npos)
                {
                    s = 1;
                }
                if (ROIs[i].params[0].find("v") != std::string::npos || ROIs[i].params[0].find("V") != std::string::npos)
                {
                    v = 1;
                }

                histoEqualizationHSV(src, tgt, h, s, v, ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
            }

			src.copyImage(tgt);
			RotateColor(src, tgt, ROIs[i], angle);
		}
	}



	//for grey images (.pgm format)
	else if (filetype == ".pgm")
	{
		for (int i = 0; i < num; i++)
		{	
			//first check what operation we are doing
			//.c_str converts from a std::string to a const char*, which is what strcasecmp uses
			if (strcasecmp(ROIs[i].op.c_str(), "addGrey") == 0)
			{
				addGrey(src, tgt, atoi(ROIs[i].params[0].c_str()), ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
			}

			else if (strcasecmp(ROIs[i].op.c_str(), "binarize") == 0)
			{
				binarize(src, tgt, atoi(ROIs[i].params[0].c_str()), ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
			}

			else if (strcasecmp(ROIs[i].op.c_str(), "valueThreshold") == 0)
			{
				valueThreshold(src, tgt, atoi(ROIs[i].params[0].c_str()), atoi(ROIs[i].params[1].c_str()), ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
			}

			else if (strcasecmp(ROIs[i].op.c_str(), "scale") == 0)
			{
				//check scale factor, if its between 1 or 2
				float scaleFactor = atof(ROIs[i].params[0].c_str());

				if (scaleFactor > 2 && scaleFactor < 1)
				{
					std::cout << "Scale factor not between 1 or 2";
					exit(1);
				}

				scale(src, tgt, scaleFactor, ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
			}

			//new functions for HW2
			else if (strcasecmp(ROIs[i].op.c_str(), "histoStretch") == 0)
			{
				histoStretch(src, tgt, fileIn, fileOut, atoi(ROIs[i].params[0].c_str()), atoi(ROIs[i].params[1].c_str()), ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
			}

            else if (strcasecmp(ROIs[i].op.c_str(), "histoEqual") == 0)
            {
                histoEqualization(src, tgt, ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
            }

            else if (strcasecmp(ROIs[i].op.c_str(), "histoEqualThreshold") == 0)
            {
                histoEqualizationThreshold(src, tgt, atoi(ROIs[i].params[0].c_str()), ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
            }

			src.copyImage(tgt);
			Rotate(src, tgt, ROIs[i], angle);
		}
	}
}