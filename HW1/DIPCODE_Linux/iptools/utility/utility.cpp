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
	//check that value is between -50 and 50
	if (value > 50 || value < -50)
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



/*-----------------------------------------------------------------------**/
/*Function for HW1, Takes in all of the ROIs, then calls the operations for each of them using the info in the ROI*/
void utility::ROI_Processing(image &src, image &tgt, std::string filetype, std::vector<ROI> ROIs, int num, int angle)
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

			if (strcasecmp(ROIs[i].op.c_str(), "multColor") == 0)
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
			src.copyImage(tgt);
			Rotate(src, tgt, ROIs[i], angle);
		}
	}
}