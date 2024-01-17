#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"

#include <iostream>

using namespace std;
using namespace cv;

int main(void) 
{
	//ocl::setUseOpenCL(true);

	VideoCapture capture(0);

	if (!capture.isOpened()) 
	{
		cout << "Can't open the camera!\n";
		return -1;
	}

	int w = cvRound(capture.get(CAP_PROP_FRAME_WIDTH));
	int h = cvRound(capture.get(CAP_PROP_FRAME_HEIGHT));
	double fps = capture.get(CAP_PROP_FPS);

	int fourcc = VideoWriter::fourcc('D', 'I', 'V', 'X');
	int delay = cvRound(1000 / 60);

	cout << delay << '\n';

	VideoWriter outputVideo;

	outputVideo.open("opt.avi", fourcc, 60.0, Size(w, h));
	
	if (!outputVideo.isOpened()) {
		cout << "File open failed!\n" << endl;
		return -1;
	}

	Mat img;

	while (1)
	{
		capture >> img;
		if (img.empty()) { break; }

		Mat dst;

		/* Use Gaussian blur */
		int sigma = 5;
		GaussianBlur(img, dst, Size(), (double)sigma);

		outputVideo << dst;

		imshow("img", dst);

		if (waitKey(30) == 27) { break; }
	}

	destroyAllWindows();

	return 0;
}