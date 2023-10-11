/************************************************************
 *															*
 * This sample project include three functions:				*
 * 1. Add intensity for gray-level image.					*
 *    Input: source image, output image name, value			*
 *															*
 * 2. Image thresholding: pixels will become black if the	*
 *    intensity is below the threshold, and white if above	*
 *    or equal the threhold.								*
 *    Input: source image, output image name, threshold		*
 *															*
 * 3. Image scaling: reduction/expansion of 2 for 			*
 *    the width and length. This project uses averaging 	*
 *    technique for reduction and pixel replication			*
 *    technique for expansion.								*
 *    Input: source image, output image name, scale factor	*
 *															*
 ************************************************************/

#include "../iptools/core.h"
#include <strings.h>
#include <string.h>

using namespace std;

#define MAXLEN 256

int main (int argc, char** argv)
{
	image src, tgt;
	FILE *fp;
	char str[MAXLEN];
	char outfile[MAXLEN];
	char *pch;
	if ((fp = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't open file: %s\n", argv[1]);
		exit(1);
	}

	while(fgets(str,MAXLEN,fp) != NULL) {
		pch = strtok(str, " ");
		src.read(pch);

		
		//get the file type
		//determine what type of file we have
		std::string ftype(pch);
		ftype = ftype.substr(ftype.length() - 4);

		pch = strtok(NULL, " ");
		strcpy(outfile, pch);

		pch = strtok(NULL, " ");

		//copy source over to target image
		//means that all of our changes are automatically saved to the target image
		tgt.copyImage(src);

		/* ROI CHANGES FOR HW1 */
		//check if we passed a valid number of ROI's
		if (atoi(pch) == 0)
		{
			std::cout << "Error: Number of ROI's is 0\n";
			exit(1);
		}

		int numROI = atoi(pch);

		//Create array of ROI objects
		std::vector<utility::ROI> ROIs;

		//get the angle to rotate all of the ROIs, must be an int: 0, 90, 180, or 270
		pch = strtok(NULL, " ");
		if (atoi(pch) == 0 && *pch != '0')
		{
			std::cout << "Error: Angle isn't a number\n";
			exit(1);
		}

		int angle = atoi(pch);

		//skip the first "ROI" we encounter
		pch = strtok(NULL, " ");

		//create the ROI's by filling in the information
		for (int i = 0; i < numROI; i++)
		{
			//the information for each ROI is as follows
			// x_coord, y_coord, size, operation, params[]
			//we delimit ROIs by putting "ROI" in front of each one in parameters.txt
			utility::ROI newROI;

			pch = strtok(NULL, " ");
			newROI.x = atoi(pch);

			pch = strtok(NULL, " ");
			newROI.y = atoi(pch);

			pch = strtok(NULL, " ");
			newROI.sx = atoi(pch);


			pch = strtok(NULL, " ");
			newROI.sy = atoi(pch);

			pch = strtok(NULL, " ");
			newROI.op = pch;

			//get the first parameter
			pch = strtok(NULL, " ");

			//read in all of the params until we hit an ROI or EOF
			while (true)
			{
				if (pch == NULL || strcmp(pch, "ROI") == 0)
				{
					break;
				}

				newROI.params.push_back(pch);
				pch = strtok(NULL, " ");
			}

			ROIs.push_back(newROI);
		}

		utility::ROI_Processing(src, tgt, ftype, ROIs, numROI, angle);

		tgt.save(outfile);

	}
	fclose(fp);

	return 0;
}

