This software is architectured as follows. This software can work on grad server.

iptools -This folder hosts the files that are compiled into a static library. 
	image - This folder hosts the files that define an image.
	utility- this folder hosts the files that students store their implemented algorithms.
	
lib- This folder hosts the static libraries associated with this software.

project- This folder hosts the files that will be compiled into executables.
	bin- This folder hosts the binary executables created in the project directory.



*** INSTALATION ***

On Linux

Enter the project directory in terminal and run make

As a result you should get iptool in project/bin directory.

*** FUNCTIONS ***

1. Add intensity: add
Increase the intensity for a gray-level image.

2. Binarization: binarize
Binarize the pixels with the threshold.

3. Scaling: Scale
Reduce or expand the heigh and width with two scale factors.
Scaling factor = 2: double height and width of the input image.
Scaling factor = 0.5: half height and width of the input image.

4. Value Thresholding: valueThreshold
New function for HW0.
Takes two parameters, a threshold and a value.
Reduces the intensity of a pixel by the value if the pixel has a lower intensity than the threshold.
The two parameters are seperated by a space, ie: baboon.pgm baboon_vt150_75.pgm valueThreshold 150 75
is how the entry should look in parameters.txt

5. Scaling: scale
New function for HW1
Takes in one parameter, a float representing the scale factor which must be between 1 and 2
Scales the image by the scale factor
Works within an ROI space, so we effectively focus on the top left corner of the ROI

6. Add Color: addColor
New function for HW1
Takes in one parameter, and int value, which represents how much brightness we want to add to the ROI
Increases the intensity of the ROI by value in each color channel

7. Multiplicative brightness: multColor
New function for HW1
Takes in one parameter, a float reprenting the scale factor that we want to increase the brightness by, must be between 1 and 2
Increases the intensity of the ROI by scale factor in each color channel

8. Histogram Stretching: histoStretch
New function for HW2
Takes in 2 parameters, 2 ints A, B which represent the range that we want to the stretch the histogram to
This range is usually 0 to 255
Performs histogram stretching on a greyscale image, and returns 3 images.
The stretched image, and 2 images of the histogram before and after we stretched

9. Histogram Equalization on grey images: histoEqual
New function for HW2
Takes in *0* parameters, and performs histogram equalization on the image, returning the stretched image
*NOTE*: Even though this function takes in no parameters, due to the way we read in parameters.txt,
we need to provide something after the operation call. Currently, in parameters.txt, we have a 0 after the
histoEqual call, although any character after the operation call will work

10. Histogram Equalization with threshold on grey images: histoEqualThreshold
New function for HW2
Takes in 1 parameter T, which represents the threshold of intensity values we want to perform histogram equalization on.

11. Histogram Equalization on RGB images: histoEqualRGB
New function for HW2
Takes in 1 parameter, a string with no spaces, which denotes the different rgb channels we want to perform histogram equalization on
ex: "histoEqualRGB r" will perform histogram equalization only on the r channel
ex: "histoEqualRGB rgb" will perform histogram equalization on all channels
*NOTE*: DO NOT put a space in the string. You may put the channel characters in any order, and case does not matter.
        ex: (RGB, bgr, gRb) all work
        ex: (R G B) will NOT work as there are spaces in between the characters

12. Histogram Equalization on HSV images: histoEqualHSV
New function for HW2
Takes in 1 parameter, a string with no spaces, which denotes the different hsv channels we want to perform histogram equalization on.
ex: "histoEqualHSV h" will perform histogram equalization only on the h channel
ex: "histoEqualHSV hsv" will perform histogram equalization on all channels
*NOTE*: DO NOT put a space in the string. You may put the channel characters in any order, and case does not matter.
        ex: (HSV, vhs, SHv) all work
        ex: (h s v) will NOT work as there are spaces in between the characters



***	TO COMPILE ***
Make sure you are using a linux environment
- Go to project and run make
- go to project/bin and run ./iptool parameters.txt



*** OPERATIONS ***

Grey methods (only .pgm format)
-	addGrey (int value)
-	binarize (int threshold)
-	valueThreshold (int threshold, int value)
-	scale (float ratio (between 1 and 2))
-   histoStretch(int A, int B)
-   histoEqual()    //NOTE: need to put something after the function call as if it were a parameter, but it won't be used.
-   histoEqualThreshold(int T)

Color methods (only .ppm format)
- 	addColor(int value)
-	multColor(float value)              //NOTE: value must be between 1 and 2
-   histoEqualRGB(string channels)      //NOTE: channels must not have any spaces. case and position of the character 'r','g','b' does not matter.
-   histoEqualHSV(string channels)      //NOTE: channels must not have any spaces. case and position of the character 'h','s','v' does not matter.



*** PARAMETERS FILE ***

You CANNOT have a newline inside each of the parameters you pass in or it will not read propely
You CAN have multiple parameter calls seperated by a newline (as seen in paramaters.txt and example 4/5)
Parameters.txt MUST end in a newline

There are 5 parameters:
1. 	the input file name;
2. 	the output file name;
3.	the number of ROIs
4.	the rotation angle for each ROI (0, 90, 180, or 270)
5.	The ROIs themselves
	- Each ROI is delimited by "ROI "
	- Each ROI is defined by:
		1. 	starting x coord
		2. 	starting y coord
		3.	size in the x direction
		4.	size in the y direction
		5.	operation name (ie addGrey, scale, multColor, etc)
		6.	a list of parameters for the given operation

*** Run the program: ./iptool parameters.txt




*** EXAMPLE PARAMETERS ****
Example 1:
	
	infile.pgm outfile.pgm 1 0 ROI 0 0 512 512 addGrey 50
	
	//	We only have 1 ROI and will not rotate it
	//	ROI is defined from 0,0 to 512,512, and we will add 50 intensity to every pixel in int


Example 2:
	infile.pgm outfile.pgm 2 90 ROI 0 0 256 256 addGrey 50 ROI 256 256 256 256 scale 1.67

	//	We have 2 ROIs not, the first one is from (0,0) to (256,256) This is the top left quadrant if the image is 512 x 512
	//	The second is from (256, 256) to (512, 512) the bottom right quadrant
	//	on the first ROI, we will add 50 brightness to every pixel in it
	//	on the second ROI, we will scale it by a factor of 1.67
	//	we will also rotate each of these ROIs by 90 degrees


Example 3:
	infile.ppm outfile.ppm 3 270 ROI 0 0 256 256 multColor 1.25 ROI 256 0 256 256 multColor 1.5 ROI 0 256 256 256 multColor 2

	//	We have 3 ROIs, and will rotate each of them by 270 degrees
	//	The first ROI calls multColor with a scale factor of 1.25
	//	The second ROI calls multColor with a scale factor of 1.5
	//	The third ROI calls multColor with a scale factor of 2


Example 4:
	infile.pgm outfile.pgm 2 90 ROI 0 0 256 256 
	addGrey 50 ROI 256 256 256 256 scale 1.67

	//	This is incorrect as there is a newline before all of the ROIs get read.
	// 	This will cause an error


Example 5:
	infile.pgm outfile_1.pgm 1 90 ROI 0 0 256 256 addGrey 25
	infile.pgm outfile_2.pgm 1 270 ROI 256 256 256 256 scale 1.56

	// This has two full paramaters, and will create 2 seperate images, outfile_1 and outfile_2
	// each one is processed seperately


Example 6:
    infile.pgm outfile.pgm 1 0 ROI 0 0 256 256 histoEqual             //incorrect

    //even though histoEqual doesn't require a parameter,
    //this will result in an error as there is nothing after the operation call.
    //we can fix this by putting in ANY character after histoEqual like so:

    infile.pgm outfile.pgm 1 0 ROI 0 0 256 256 histoEqual 0           //correct


Example 7:
    infile.ppm outfile.ppm 1 0 ROI 0 0 256 256 histoEqualRGB r g b      //incorrect
    infile.ppm outfile.ppm 1 0 ROI 0 0 256 256 histoEqualHSV h s v      //incorrect

    //no spaces are allowed in the channel arguments, do this instead

    infile.ppm outfile.ppm 1 0 ROI 0 0 256 256 histoEqualRGB rgb      //correct
    infile.ppm outfile.ppm 1 0 ROI 0 0 256 256 histoEqualHSV hsv      //correct

    //case and position do not matter, so the following are correct as well
    infile.ppm outfile.ppm 1 0 ROI 0 0 256 256 histoEqualRGB GBR      //correct
    infile.ppm outfile.ppm 1 0 ROI 0 0 256 256 histoEqualHSV vHs      //correct
