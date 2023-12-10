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














/*Histogram image function.
 *Creates an image of a histogram and saves it. used in histoStretch*/
void matToImage(cv::Mat dft, std::string filename)
{
    image img(dft.rows, dft.cols);

    //for every element in the dft Mat, copy the value to the img
    //making sure to normalize by multiplying by 255
    //copy values from out into the tgt image
    for (int i = 0; i < dft.rows; i++)
    {
        for (int j = 0; j < dft.cols; j++)
        {
            //since dft is float based, use float then convert to int
            img.setPixel(i, j, (int)dft.at<uchar>(j,i));
        }
    }

    img.save(filename.c_str());
}



//creates a filter with a circle with a specified radius
//the "inside" of the circle has one value, and the outside another.
//takes in the size of the magnitude spectrum image
cv::Mat circleFilter(int rows, int cols, int radius, float inside, float outside)
{
    //create image representing filter
    cv::Mat filter = cv::Mat(rows, cols, CV_32F);

    //find the center of the filter
    int centerX = rows / 2;
    int centerY = cols / 2;
    float distance;
    //for each pixel in the filter
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            //find the distance from the center to this point
            distance = sqrt(pow((i - centerX), 2.0) + pow((j - centerY), 2.0));

            //if we are inside the circle
            if (distance < radius)
            {
                filter.at<float>(i, j) = inside;
            }

            else
            {
                filter.at<float>(i, j) = outside;
            }
        }
    }

    //return the filter
    return filter;
}


void rearrangeMat(cv::Mat &magI)
{
    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));
    // rearrange the quadrants of Fourier image so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;
    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy)); // Top-Left - Create a ROI per quadrant
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy)); // Top-Right
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy)); // Bottom-Left
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right
    cv::Mat tmp; // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp); // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
}



void utility::high_low_pass(image &src, image &tgt, std::string fileIn, std::string fileOut, int radius, int inside, int outside, int x, int y, int sx, int sy)
{
    //create cv::Mat containing the ROI image
    cv::Mat in = cv::Mat(sx, sx, CV_8UC1);

    //loop through ROI, and set each pixel in the mat to the value, filling it
    for (int i = x; i < x + sx; i++)
    {
        for (int j = y; j < y + sy; j++)
        {
            in.at<uchar>(j-y, i-x) = tgt.getPixel(i, j);
        }
    }

    //since the result of the fourier transform is complex, we need to Mats to store the result (one mat for each component)
    //also, freq domain has a much larger range than the spatial one does, so we need to represent it with floats

    cv::Mat padded;                            //expand input image to optimal size
    int m = cv::getOptimalDFTSize( in.rows );
    int n = cv::getOptimalDFTSize( in.cols ); // on the border add zero values
    cv::copyMakeBorder(in, padded, 0, m - in.rows, 0, n - in.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    //planes holds two mats, the first represents our actual input image
    //the second is a blank float mat which will store the complex part of the dft
    cv::Mat temp;
    padded.convertTo(temp, CV_32F);
    cv::Mat planes[] = {temp, cv::Mat::zeros(padded.size(), CV_32F)};

    //merge planes into one mat, dftImage
    cv::Mat dftImage;
    cv::merge(planes, 2, dftImage);

    //perform dft
    cv::dft(dftImage, dftImage);

    //split the dft image into real and imaginary parts
    cv::split(dftImage, planes); // planes[0] = Real(DFT(I), planes[1] = Imaginary(DFT(I))

    //store the magnitude of the two parts in planes[0]
    cv::magnitude(planes[0], planes[1], planes[0]);

    //create a new Mat out of the magnitude
    cv::Mat magImg = planes[0];

    //scale and rearrange our magI mat so that we can properly display it
    magImg += cv::Scalar::all(1); // switch to logarithmic scale, as float range is far to big to display
    cv::log(magImg, magImg);
    rearrangeMat(magImg);

    //create image of magnitude spectrum
    // Transform the matrix with float values into a
    // viewable image form (uchar (int) between values 0 and 255).
    cv::normalize(magImg, magImg, 0, 255, cv::NORM_MINMAX, CV_8U);
    fileOut = fileOut + "_" + std::to_string(x) + "_" + std::to_string(y);
    std::string magImgStr = fileOut + "_FrequencyMap_Before.pgm";
    matToImage(magImg , magImgStr);


    //create filter, a single black/white image with a circle of size radius
    cv::Mat filter = circleFilter(padded.rows, padded.cols, radius, static_cast<float>(inside), static_cast<float>(outside));

    //Used to display filter image
    cv::Mat filterCopy = filter.clone();
    cv::normalize(filterCopy, filterCopy, 0, 255, cv::NORM_MINMAX, CV_8U);
    std::string filterImgStr = fileOut + "_filter.pgm";
    matToImage(filterCopy, filterImgStr);


    //apply filter to magImg by doing element wise multiplication with the filter
    //need to create 2 channel filter
    //use dftImg as we need real and imaginary spectrum
    //clone it so it is the same size as our filtersTemp (2 2d Arrays)
    cv::Mat filtersTemp[] = {filter, filter};
    cv::Mat clone = dftImage.clone();
    cv::merge(filtersTemp, 2, clone);

    //center dft:
    rearrangeMat(dftImage);

    //element wise multiplication of the dftImage with the filters
    cv::mulSpectrums(dftImage, clone, dftImage, 0); //apply complex-element wise multiplication

    //find the mag spectrum of dftImage to output the filtered spectrum
    cv::split(dftImage, planes); // planes[0] = Real(DFT(I), planes[1] = Imaginary(DFT(I))
    //store the magnitude of the two parts in planes[0]
    cv::magnitude(planes[0], planes[1], planes[0]);
    //create a new Mat out of the magnitude
    cv::Mat filteredDFT = planes[0];
    //scale and rearrange our filteredDFT mat so that we can properly display it
    filteredDFT += cv::Scalar::all(1); // switch to logarithmic scale, as float range is far to big to display
    cv::log(filteredDFT, filteredDFT);

    //display this new filtered image
    cv::normalize(filteredDFT, filteredDFT, 0, 255, cv::NORM_MINMAX, CV_8U);
    std::string filteredImgStr = fileOut + "_FrequencyMap_After.pgm";
    matToImage(filteredDFT, filteredImgStr);


    //uncenter the dft
    rearrangeMat(dftImage);

    //perform the inverse fourier transform
    cv::idft(dftImage, dftImage);

    //split the idft image into real and imaginary parts
    cv::split(dftImage, planes);

    cv::Mat outImage;
    cv::normalize(planes[0], outImage, 0, 255, cv::NORM_MINMAX, CV_8U);

    //copy values from out into the tgt image
    for (int i = x; i < x + sx; i++)
    {
        for (int j = y; j < y + sy; j++)
        {
            tgt.setPixel(i, j, (int)outImage.at<uchar>(j-y,i-x));
        }
    }
}



//basically just does low/high pass filter from an input mat without all the additional steps
cv::Mat unsharpMaskingHelper(cv::Mat in, int radius, int inside, int outside)
{
    cv::Mat padded;                            //expand input image to optimal size
    int m = cv::getOptimalDFTSize( in.rows );
    int n = cv::getOptimalDFTSize( in.cols ); // on the border add zero values
    cv::copyMakeBorder(in, padded, 0, m - in.rows, 0, n - in.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    //planes holds two mats, the first represents our actual input image
    //the second is a blank float mat which will store the complex part of the dft
    cv::Mat temp;
    padded.convertTo(temp, CV_32F);
    cv::Mat planes[] = {temp, cv::Mat::zeros(padded.size(), CV_32F)};

    //merge planes into one mat, dftImage
    cv::Mat dftImage;
    cv::merge(planes, 2, dftImage);

    //perform dft
    cv::dft(dftImage, dftImage);

    //create filter, a single black/white image with a circle of size radius
    cv::Mat filter = circleFilter(padded.rows, padded.cols, radius, static_cast<float>(inside), static_cast<float>(outside));
    cv::Mat filtersTemp[] = {filter, filter};
    cv::Mat clone = dftImage.clone();
    cv::merge(filtersTemp, 2, clone);

    //center dft:
    rearrangeMat(dftImage);

    //element wise multiplication of the dftImage with the filters
    cv::mulSpectrums(dftImage, clone, dftImage, 0); //apply complex-element wise multiplication

    //return both frequency and imaginary domain
    return dftImage;
}



/* Our unsharp masking function
 * finalImage = lowpass + ((1 + alpha) * highpass)*/
void utility::unsharpMasking(image &src, image &tgt, std::string fileIn, std::string fileOut, int radius, float alpha, int x, int y, int sx, int sy)
{
    //create cv::Mat containing the ROI image
    cv::Mat in = cv::Mat(sx, sx, CV_8UC1);

    //loop through ROI, and set each pixel in the mat to the value, filling it
    for (int i = x; i < x + sx; i++)
    {
        for (int j = y; j < y + sy; j++)
        {
            in.at<uchar>(j-y, i-x) = tgt.getPixel(i, j);
        }
    }

    cv::Mat padded;                            //expand input image to optimal size
    int m = cv::getOptimalDFTSize( in.rows );
    int n = cv::getOptimalDFTSize( in.cols ); // on the border add zero values
    cv::copyMakeBorder(in, padded, 0, m - in.rows, 0, n - in.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    //planes holds two mats, the first represents our actual input image
    //the second is a blank float mat which will store the complex part of the dft
    cv::Mat temp;
    padded.convertTo(temp, CV_32F);
    cv::Mat planes[] = {temp, cv::Mat::zeros(padded.size(), CV_32F)};

    //merge planes into one mat, dftImage
    cv::Mat dftImage;
    cv::merge(planes, 2, dftImage);

    //perform dft
    cv::dft(dftImage, dftImage);

    //center dft
    rearrangeMat(dftImage);

    //create a copy of in to blur it
    cv::Mat lowpass = in.clone();

    //perform a lowpass filter on the clone using the provided radius
    //need to change highlowPass function to accept a Mat and return it
    //or create a very simplified helper function that does the same thing
    lowpass = unsharpMaskingHelper(lowpass, radius, 1, 0);

    //Used to display filter image
    cv::Mat lowpassCopy = lowpass.clone();
    cv::split(lowpassCopy, planes); // planes[0] = Real(DFT(I), planes[1] = Imaginary(DFT(I))
    cv::magnitude(planes[0], planes[1], planes[0]);
    lowpassCopy = planes[0];
    lowpassCopy += cv::Scalar::all(1); // switch to logarithmic scale, as float range is far to big to display
    cv::log(lowpassCopy, lowpassCopy);
    cv::normalize(lowpassCopy, lowpassCopy, 0, 255, cv::NORM_MINMAX, CV_8U);
    fileOut = fileOut + "_" + std::to_string(x) + "_" + std::to_string(y);
    std::string lowpassImgStr = fileOut + "_lowpassPortion.pgm";
    matToImage(lowpassCopy, lowpassImgStr);

    cv::Mat highpass = in.clone();
    highpass = unsharpMaskingHelper(highpass, radius, 0, 1);
    cv::multiply(highpass, 1.0 + alpha, highpass);

    //used to display highpass filter
    cv::Mat highpassCopy = highpass.clone();
    cv::split(highpassCopy, planes); // planes[0] = Real(DFT(I), planes[1] = Imaginary(DFT(I))
    cv::magnitude(planes[0], planes[1], planes[0]);
    highpassCopy = planes[0];
    highpassCopy += cv::Scalar::all(1); // switch to logarithmic scale, as float range is far to big to display
    cv::log(highpassCopy, highpassCopy);
    cv::normalize(highpassCopy, highpassCopy, 0, 255, cv::NORM_MINMAX, CV_8U);
    std::string highpassImgStr = fileOut + "_highpassPortion.pgm";
    matToImage(highpassCopy, highpassImgStr);

    cv::Mat outImage;
    cv::add(lowpass, highpass, outImage);

    //used to display output filter
    cv::Mat outImageCopy = outImage.clone();
    cv::split(outImageCopy, planes); // planes[0] = Real(DFT(I), planes[1] = Imaginary(DFT(I))
    cv::magnitude(planes[0], planes[1], planes[0]);
    outImageCopy = planes[0];
    outImageCopy += cv::Scalar::all(1); // switch to logarithmic scale, as float range is far to big to display
    cv::log(outImageCopy, outImageCopy);
    cv::normalize(outImageCopy, outImageCopy, 0, 255, cv::NORM_MINMAX, CV_8U);
    fileOut = fileOut + "_" + std::to_string(x) + "_" + std::to_string(y);
    std::string outImageImgStr = fileOut + "_outImagePortion.pgm";
    matToImage(outImageCopy, outImageImgStr);


    rearrangeMat(outImage);
    cv::idft(outImage, outImage);

    //split the outImage image into real and imaginary parts
    cv::split(outImage, planes);
    cv::normalize(planes[0], outImage, 0, 255, cv::NORM_MINMAX, CV_8U);

    //copy values from out into the tgt image
    for (int i = x; i < x + sx; i++)
    {
        for (int j = y; j < y + sy; j++)
        {
            tgt.setPixel(i, j, (int)outImage.at<uchar>(j-y,i-x));
        }
    }
}



//bandstop Filter function
//essentially the same as circle filter but with a different equation using both radii
cv::Mat bandstopFilter(int rows, int cols, int radiusA, int radiusB)
{
    //create image representing filter
    cv::Mat filter = cv::Mat(rows, cols, CV_32F);

    //find the center of the filter
    int centerX = rows / 2;
    int centerY = cols / 2;
    float distance;
    //for each pixel in the filter
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            //find the distance from the center to this point
            distance = sqrt(pow((i - centerX), 2.0) + pow((j - centerY), 2.0));

            //if we are inside the first radius, or outside the second, set it to black
            if (distance < radiusA || distance > radiusB)
            {
                filter.at<float>(i, j) = 0.0;
            }

            else
            {
                filter.at<float>(i, j) = 1.0;
            }
        }
    }

    //return the filter
    return filter;
}

/* Bandstop function
 * works essentially the same as our highlow pass funciton, but with a different filter*/
void utility::bandstop(image &src, image &tgt, std::string fileIn, std::string fileOut, int radiusA, int radiusB, int x, int y, int sx, int sy)
{
    //create cv::Mat containing the ROI image
    cv::Mat in = cv::Mat(sx, sx, CV_8UC1);

    //loop through ROI, and set each pixel in the mat to the value, filling it
    for (int i = x; i < x + sx; i++)
    {
        for (int j = y; j < y + sy; j++)
        {
            in.at<uchar>(j-y, i-x) = tgt.getPixel(i, j);
        }
    }

    //since the result of the fourier transform is complex, we need to Mats to store the result (one mat for each component)
    //also, freq domain has a much larger range than the spatial one does, so we need to represent it with floats

    cv::Mat padded;                            //expand input image to optimal size
    int m = cv::getOptimalDFTSize( in.rows );
    int n = cv::getOptimalDFTSize( in.cols ); // on the border add zero values
    cv::copyMakeBorder(in, padded, 0, m - in.rows, 0, n - in.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    //planes holds two mats, the first represents our actual input image
    //the second is a blank float mat which will store the complex part of the dft
    cv::Mat temp;
    padded.convertTo(temp, CV_32F);
    cv::Mat planes[] = {temp, cv::Mat::zeros(padded.size(), CV_32F)};


    //merge planes into one mat, dftImage
    cv::Mat dftImage;
    cv::merge(planes, 2, dftImage);

    //perform dft
    cv::dft(dftImage, dftImage);

    //split the dft image into real and imaginary parts
    cv::split(dftImage, planes); // planes[0] = Real(DFT(I), planes[1] = Imaginary(DFT(I))

    //store the magnitude of the two parts in planes[0]
    cv::magnitude(planes[0], planes[1], planes[0]);

    //create a new Mat out of the magnitude
    cv::Mat magImg = planes[0];

    //scale and rearrange our magI mat so that we can properly display it
    magImg += cv::Scalar::all(1); // switch to logarithmic scale, as float range is far to big to display
    cv::log(magImg, magImg);
    rearrangeMat(magImg);

    //create image of magnitude spectrum
    // Transform the matrix with float values into a
    // viewable image form (uchar (int) between values 0 and 255).
    cv::normalize(magImg, magImg, 0, 255, cv::NORM_MINMAX, CV_8U);
    fileOut = fileOut + "_" + std::to_string(x) + "_" + std::to_string(y);
    std::string magImgStr = fileOut + "_FrequencyMap_Before.pgm";
    matToImage(magImg , magImgStr);


    //create filter, a single black/white image with a circle of size radius
    cv::Mat filter = bandstopFilter(padded.rows, padded.cols, radiusA, radiusB);

    //Used to display filter image
    cv::Mat filterCopy = filter.clone();
    cv::normalize(filterCopy, filterCopy, 0, 255, cv::NORM_MINMAX, CV_8U);
    std::string filterImgStr = fileOut + "_filter.pgm";
    matToImage(filterCopy, filterImgStr);


    //apply filter to magImg by doing element wise multiplication with the filter
    //need to create 2 channel filter
    //use dftImg as we need real and imaginary spectrum
    //clone it so it is the same size as our filtersTemp (2 2d Arrays)
    cv::Mat filtersTemp[] = {filter, filter};
    cv::Mat clone = dftImage.clone();
    cv::merge(filtersTemp, 2, clone);

    //center dft:
    rearrangeMat(dftImage);

    //element wise multiplication of the dftImage with the filters
    cv::mulSpectrums(dftImage, clone, dftImage, 0); //apply complex-element wise multiplication


    //find the mag spectrum of dftImage to output the filtered spectrum
    cv::split(dftImage, planes); // planes[0] = Real(DFT(I), planes[1] = Imaginary(DFT(I))
    //store the magnitude of the two parts in planes[0]
    cv::magnitude(planes[0], planes[1], planes[0]);
    //create a new Mat out of the magnitude
    cv::Mat filteredDFT = planes[0];
    //scale and rearrange our magI mat so that we can properly display it
    filteredDFT += cv::Scalar::all(1); // switch to logarithmic scale, as float range is far to big to display
    cv::log(filteredDFT, filteredDFT);

    //display this new filtered image
    cv::normalize(filteredDFT, filteredDFT, 0, 255, cv::NORM_MINMAX, CV_8U);
    std::string filteredImgStr = fileOut + "_FrequencyMap_After.pgm";
    matToImage(filteredDFT, filteredImgStr);


    //uncenter the dft and perform idft
    rearrangeMat(dftImage);
    cv::idft(dftImage, dftImage);

    //split the idft image into real and imaginary parts
    cv::split(dftImage, planes);

    cv::Mat outImage;
    cv::normalize(planes[0], outImage, 0, 255, cv::NORM_MINMAX, CV_8U);

    //copy values from out into the tgt image
    for (int i = x; i < x + sx; i++)
    {
        for (int j = y; j < y + sy; j++)
        {
            tgt.setPixel(i, j, (int)outImage.at<uchar>(j-y,i-x));
        }
    }
}



cv::Mat high_low_pass_hsv_helper(cv::Mat in, std::string fileIn, std::string fileOut, int x, int y, int radius, int inside, int outside, int channel)
{
    //since the result of the fourier transform is complex, we need to Mats to store the result (one mat for each component)
    //also, freq domain has a much larger range than the spatial one does, so we need to represent it with floats

    cv::Mat padded;                            //expand input image to optimal size
    int m = cv::getOptimalDFTSize( in.rows );
    int n = cv::getOptimalDFTSize( in.cols ); // on the border add zero values
    cv::copyMakeBorder(in, padded, 0, m - in.rows, 0, n - in.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    //planes holds two mats, the first represents our actual input image
    //the second is a blank float mat which will store the complex part of the dft
    cv::Mat temp;
    padded.convertTo(temp, CV_32F);
    cv::Mat planes[] = {temp, cv::Mat::zeros(padded.size(), CV_32F)};


    //merge planes into one mat, dftImage
    cv::Mat dftImage;
    cv::merge(planes, 2, dftImage);

    //perform dft
    cv::dft(dftImage, dftImage);

    //split the dft image into real and imaginary parts
    cv::split(dftImage, planes); // planes[0] = Real(DFT(I), planes[1] = Imaginary(DFT(I))

    //store the magnitude of the two parts in planes[0]
    cv::magnitude(planes[0], planes[1], planes[0]);

    //create a new Mat out of the magnitude
    cv::Mat magImg = planes[0];

    //scale and rearrange our magI mat so that we can properly display it
    magImg += cv::Scalar::all(1); // switch to logarithmic scale, as float range is far to big to display
    cv::log(magImg, magImg);
    rearrangeMat(magImg);

    //create image of magnitude spectrum
    // Transform the matrix with float values into a
    // viewable image form (uchar (int) between values 0 and 255).

    //if channel == 0, this is hsv which is normalized from 0 to 179, NOT 255
    if (channel == 0)
    {
        cv::normalize(magImg, magImg, 0, 179, cv::NORM_MINMAX, CV_8U);
    }
    else
    {
        cv::normalize(magImg, magImg, 0, 255, cv::NORM_MINMAX, CV_8U);
    }
    fileOut = fileOut + "_" + std::to_string(x) + "_" + std::to_string(y);
    std::string magImgStr = fileOut + "_HSV_FrequencyMap_Before.pgm";
    matToImage(magImg , magImgStr);


    //create filter, a single black/white image with a circle of size radius
    cv::Mat filter = circleFilter(padded.rows, padded.cols, radius, static_cast<float>(inside), static_cast<float>(outside));

    //Used to display filter image
    cv::Mat filterCopy = filter.clone();
    cv::normalize(filterCopy, filterCopy, 0, 255, cv::NORM_MINMAX, CV_8U);
    std::string filterImgStr = fileOut + "_HSV_filter.pgm";
    matToImage(filterCopy, filterImgStr);


    //apply filter to magImg by doing element wise multiplication with the filter
    //need to create 2 channel filter
    //use dftImg as we need real and imaginary spectrum
    //clone it so it is the same size as our filtersTemp (2 2d Arrays)
    cv::Mat filtersTemp[] = {filter, filter};
    cv::Mat clone = dftImage.clone();
    cv::merge(filtersTemp, 2, clone);

    //center dft:
    rearrangeMat(dftImage);

    //element wise multiplication of the dftImage with the filters
    cv::mulSpectrums(dftImage, clone, dftImage, 0); //apply complex-element wise multiplication


    //find the mag spectrum of dftImage to output the filtered spectrum
    cv::split(dftImage, planes); // planes[0] = Real(DFT(I), planes[1] = Imaginary(DFT(I))
    //store the magnitude of the two parts in planes[0]
    cv::magnitude(planes[0], planes[1], planes[0]);
    //create a new Mat out of the magnitude
    cv::Mat filteredDFT = planes[0];
    //scale and rearrange our magI mat so that we can properly display it
    filteredDFT += cv::Scalar::all(1); // switch to logarithmic scale, as float range is far to big to display
    cv::log(filteredDFT, filteredDFT);


    //display this new filtered image
    cv::normalize(filteredDFT, filteredDFT, 0, 255, cv::NORM_MINMAX, CV_8U);
    if (channel == 0)
    {
        cv::normalize(filteredDFT, filteredDFT, 0, 179, cv::NORM_MINMAX, CV_8U);
    }
    else
    {
        cv::normalize(filteredDFT, filteredDFT, 0, 255, cv::NORM_MINMAX, CV_8U);
    }
    std::string filteredImgStr = fileOut + "_HSV_FrequencyMap_After.pgm";
    matToImage(filteredDFT, filteredImgStr);


    //uncenter the dft and perform idft
    rearrangeMat(dftImage);
    cv::idft(dftImage, dftImage);

    //split the idft image into real and imaginary parts
    cv::split(dftImage, planes);

    cv::Mat outImage;
    if (channel == 0)
    {
        cv::normalize(planes[0], outImage, 0, 179, cv::NORM_MINMAX, CV_8U);
    }
    else
    {
        cv::normalize(planes[0], outImage, 0, 255, cv::NORM_MINMAX, CV_8U);
    }

    return outImage;
}



//hsv high/low pass function
void utility::high_low_pass_hsv(image &src, image &tgt, std::string fileIn, std::string fileOut, int radius, int inside, int outside, int H, int S, int V, int x, int y, int sx, int sy)
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

    if (H == 1)
    {
        channels[0] = high_low_pass_hsv_helper(channels[0], fileIn, fileOut, x, y, radius, inside, outside, 0);
    }

    if (S == 1)
    {
        channels[1] = high_low_pass_hsv_helper(channels[1], fileIn, fileOut, x, y, radius, inside, outside, 1);
    }

    if (V == 1)
    {
        channels[2] = high_low_pass_hsv_helper(channels[2], fileIn, fileOut, x, y, radius, inside, outside, 2);
    }

    //merge channels back together to get complete image. save it in equalized
    cv::Mat filtered;
    cv::merge(channels, filtered);

    //convert equalized back to rgb for display, saved in out
    cv::Mat out;
    cv::cvtColor(filtered, out, cv::COLOR_HSV2BGR);

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



cv::Mat bandstop_hsv_helper(cv::Mat in, std::string fileIn, std::string fileOut, int radiusA, int radiusB, int x, int y, int channel)
{
    //since the result of the fourier transform is complex, we need to Mats to store the result (one mat for each component)
    //also, freq domain has a much larger range than the spatial one does, so we need to represent it with floats

    cv::Mat padded;                            //expand input image to optimal size
    int m = cv::getOptimalDFTSize( in.rows );
    int n = cv::getOptimalDFTSize( in.cols ); // on the border add zero values
    cv::copyMakeBorder(in, padded, 0, m - in.rows, 0, n - in.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    //planes holds two mats, the first represents our actual input image
    //the second is a blank float mat which will store the complex part of the dft
    cv::Mat temp;
    padded.convertTo(temp, CV_32F);
    cv::Mat planes[] = {temp, cv::Mat::zeros(padded.size(), CV_32F)};


    //merge planes into one mat, dftImage
    cv::Mat dftImage;
    cv::merge(planes, 2, dftImage);

    //perform dft
    cv::dft(dftImage, dftImage);

    //split the dft image into real and imaginary parts
    cv::split(dftImage, planes); // planes[0] = Real(DFT(I), planes[1] = Imaginary(DFT(I))

    //store the magnitude of the two parts in planes[0]
    cv::magnitude(planes[0], planes[1], planes[0]);

    //create a new Mat out of the magnitude
    cv::Mat magImg = planes[0];

    //scale and rearrange our magI mat so that we can properly display it
    magImg += cv::Scalar::all(1); // switch to logarithmic scale, as float range is far to big to display
    cv::log(magImg, magImg);
    rearrangeMat(magImg);

    //create image of magnitude spectrum
    // Transform the matrix with float values into a
    // viewable image form (uchar (int) between values 0 and 255).

    //if channel == 0, this is the h channel which is normalized from 0 to 179, NOT 255
    if (channel == 0)
    {
        cv::normalize(magImg, magImg, 0, 179, cv::NORM_MINMAX, CV_8U);
    }
    else
    {
        cv::normalize(magImg, magImg, 0, 255, cv::NORM_MINMAX, CV_8U);
    }

    fileOut = fileOut + "_" + std::to_string(x) + "_" + std::to_string(y);
    std::string magImgStr = fileOut + "_HSV_FrequencyMap_Before.pgm";
    matToImage(magImg , magImgStr);


    //create filter, a single black/white image with a circle of size radius
    cv::Mat filter = bandstopFilter(padded.rows, padded.cols, radiusA, radiusB);

    //Used to display filter image
    cv::Mat filterCopy = filter.clone();
    cv::normalize(filterCopy, filterCopy, 0, 255, cv::NORM_MINMAX, CV_8U);
    std::string filterImgStr = fileOut + "_HSV_filter.pgm";
    matToImage(filterCopy, filterImgStr);


    //apply filter to magImg by doing element wise multiplication with the filter
    //need to create 2 channel filter
    //use dftImg as we need real and imaginary spectrum
    //clone it so it is the same size as our filtersTemp (2 2d Arrays)
    cv::Mat filtersTemp[] = {filter, filter};
    cv::Mat clone = dftImage.clone();
    cv::merge(filtersTemp, 2, clone);

    //center dft:
    rearrangeMat(dftImage);

    //element wise multiplication of the dftImage with the filters
    cv::mulSpectrums(dftImage, clone, dftImage, 0); //apply complex-element wise multiplication


    //find the mag spectrum of dftImage to output the filtered spectrum
    cv::split(dftImage, planes); // planes[0] = Real(DFT(I), planes[1] = Imaginary(DFT(I))
    //store the magnitude of the two parts in planes[0]
    cv::magnitude(planes[0], planes[1], planes[0]);
    //create a new Mat out of the magnitude
    cv::Mat filteredDFT = planes[0];
    //scale and rearrange our magI mat so that we can properly display it
    filteredDFT += cv::Scalar::all(1); // switch to logarithmic scale, as float range is far to big to display
    cv::log(filteredDFT, filteredDFT);


    //display this new filtered image
    cv::normalize(filteredDFT, filteredDFT, 0, 255, cv::NORM_MINMAX, CV_8U);
    if (channel == 0)
    {
        cv::normalize(filteredDFT, filteredDFT, 0, 179, cv::NORM_MINMAX, CV_8U);
    }
    else
    {
        cv::normalize(filteredDFT, filteredDFT, 0, 255, cv::NORM_MINMAX, CV_8U);
    }
    std::string filteredImgStr = fileOut + "_HSV_FrequencyMap_After.pgm";
    matToImage(filteredDFT, filteredImgStr);


    //uncenter the dft and perform idft
    rearrangeMat(dftImage);
    cv::idft(dftImage, dftImage);

    //split the idft image into real and imaginary parts
    cv::split(dftImage, planes);

    cv::Mat outImage;
    if (channel == 0)
    {
        cv::normalize(planes[0], outImage, 0, 179, cv::NORM_MINMAX, CV_8U);
    }
    else
    {
        cv::normalize(planes[0], outImage, 0, 255, cv::NORM_MINMAX, CV_8U);
    }

    return outImage;
}


/* Function for HW3 - Bandpass filter on HSV channels */
void utility::bandstop_hsv(image &src, image &tgt, std::string fileIn, std::string fileOut, int radiusA, int radiusB, int H, int S, int V, int x, int y, int sx, int sy)
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

    if (H == 1)
    {
        channels[0] = bandstop_hsv_helper(channels[0], fileIn, fileOut, radiusA, radiusB, x, y, 0);
    }

    if (S == 1)
    {
        channels[1] = bandstop_hsv_helper(channels[1], fileIn, fileOut, radiusA, radiusB, x, y, 1);
    }

    if (V == 1)
    {
        channels[2] = bandstop_hsv_helper(channels[2], fileIn, fileOut, radiusA, radiusB, x, y, 2);
    }

    //merge channels back together to get complete image. save it in equalized
    cv::Mat filtered;
    cv::merge(channels, filtered);

    //convert equalized back to rgb for display, saved in out
    cv::Mat out;
    cv::cvtColor(filtered, out, cv::COLOR_HSV2BGR);

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

            //does high/low pass filter on HSV channels specified by user
            else if ((strcasecmp(ROIs[i].op.c_str(), "highpass") == 0) || (strcasecmp(ROIs[i].op.c_str(), "lowpass") == 0))
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

                if (strcasecmp(ROIs[i].op.c_str(), "highpass") == 0)
                {
                    high_low_pass_hsv(src, tgt, fileIn, fileOut, atoi(ROIs[i].params[1].c_str()), 0, 1, h, s, v, ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
                }

                if (strcasecmp(ROIs[i].op.c_str(), "lowpass") == 0)
                {
                    high_low_pass_hsv(src, tgt, fileIn, fileOut, atoi(ROIs[i].params[1].c_str()), 1, 0, h, s, v, ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
                }
            }

            //does bandpass filter on HSV channels specified by user
            else if (strcasecmp(ROIs[i].op.c_str(), "bandstop") == 0)
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

                bandstop_hsv(src, tgt, fileIn, fileOut, atoi(ROIs[i].params[1].c_str()), atoi(ROIs[i].params[2].c_str()), h, s, v, ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
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

            //functions for HW3
            //both high and lowpass use the same function.
            // highpass has the inside of the circle = 0 and outside = 1, lowpass is the opposite
            else if (strcasecmp(ROIs[i].op.c_str(), "highpass") == 0)
            {
                high_low_pass(src, tgt, fileIn, fileOut, atoi(ROIs[i].params[0].c_str()), 0, 1, ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
            }

            else if (strcasecmp(ROIs[i].op.c_str(), "lowpass") == 0)
            {
                high_low_pass(src, tgt, fileIn, fileOut, atoi(ROIs[i].params[0].c_str()), 1, 0, ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
            }

            else if (strcasecmp(ROIs[i].op.c_str(), "unsharpMasking") == 0)
            {
                unsharpMasking(src, tgt, fileIn, fileOut, atoi(ROIs[i].params[0].c_str()), atof(ROIs[i].params[1].c_str()), ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
            }

            else if (strcasecmp(ROIs[i].op.c_str(), "bandstop") == 0)
            {
                bandstop(src, tgt, fileIn, fileOut, atoi(ROIs[i].params[0].c_str()), atoi(ROIs[i].params[1].c_str()), ROIs[i].x, ROIs[i].y, ROIs[i].sx, ROIs[i].sy);
            }

			src.copyImage(tgt);
			Rotate(src, tgt, ROIs[i], angle);
		}
	}
}