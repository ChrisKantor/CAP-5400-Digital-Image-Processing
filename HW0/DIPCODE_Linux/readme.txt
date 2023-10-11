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




*** PARAMETERS FILE ***
	
There are five parameters:
1. 	the input file name;
2. 	the output file name;
3. 	the name of the filter. Use "add", "binarize", and "scale" for your filters;
4. 	the value for adding intensity, threshold value for binarize filter, the scaling factor for scale filter,
   	or the threshold  for the valueThreshold filter.
5.	the value for the valueThreshold filter. Leave this parameter empty if you are not using the valueThreshold filter.


*** Run the program: ./iptool parameters.txt
