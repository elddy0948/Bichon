#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"

#include <iostream>

using namespace std;
using namespace cv;

int main(void) 
{
	ocl::setUseOpenCL(true);

	VideoCapture capture(0);

	if (!capture.isOpened()) 
	{
		cout << "Can't open the camera!\n";
		return -1;
	}

	UMat img;
	while (1)
	{
		capture >> img;
		UMat dst;

		/* Use Gaussian blur */
		int sigma = 5;
		GaussianBlur(img, dst, Size(), (double)sigma);
		imshow("img", dst);

		if (waitKey(1) == 27) { break; }
	}

	destroyAllWindows();

	return 0;
}