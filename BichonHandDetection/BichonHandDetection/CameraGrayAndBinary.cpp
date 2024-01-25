#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

const String originalName = "original";
const String grayName = "gray";
const String binaryName = "binary";

bool once = false;

Mat originalImage;
Mat grayscaleImage;
Mat binaryImage;
int g_pos = 128;

void on_threshold(int pos, void* userdata);

int main(void)
{
	VideoCapture cap(0);


	while (1)
	{
		cap >> originalImage;
		cvtColor(originalImage, grayscaleImage, COLOR_BGR2GRAY);

		namedWindow(originalName);
		namedWindow(grayName);
		namedWindow(binaryName);

		imshow(originalName, originalImage);
		imshow(grayName, grayscaleImage);

		if (!once)
		{
			if (!grayscaleImage.empty())
			{
				createTrackbar("Threshold", binaryName, 0, 255, on_threshold, (void*)&grayscaleImage);
				setTrackbarPos("Threshold", binaryName, 128);
				once = true;
			}
		}

		threshold(grayscaleImage, binaryImage, g_pos, 255, THRESH_BINARY);
		imshow(binaryName, binaryImage);

		if (waitKey(10) == 27)
			break;
	}

	destroyAllWindows();

	return 0;
}

void on_threshold(int pos, void* userdata)
{
	Mat src = *(Mat*)userdata;
	g_pos = pos;
}
