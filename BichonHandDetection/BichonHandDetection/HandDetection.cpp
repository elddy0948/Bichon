#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

/* 카메라에 손만 나와야 합니다.. ㅎㅎ */

const String resultName = "result";
const String fileName = "hand_image.bmp";
const String colorFile = "hand_color.bmp";
const String filteredWindow = "Filtered";
const String savedWindow = "Saved";
const String loadedWindow = "Loaded";

Mat originalImage;
Mat capturedImage;
Mat capturedColorImage;
Mat filteredImage;

void capture_image()
{
	imwrite(fileName, filteredImage);
	imwrite(colorFile, originalImage);

	imshow(savedWindow, filteredImage);
}

void load_image()
{
	capturedImage = imread(fileName, IMREAD_GRAYSCALE);
	capturedColorImage = imread(colorFile, IMREAD_COLOR);

	imshow(loadedWindow, capturedImage);
}

void findcts()
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(capturedImage, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); ++i)
	{
		Scalar c(rand() & 255, rand() & 255, rand() & 255);
		drawContours(capturedColorImage, contours, i, c, -1, LINE_8, hierarchy);
	}

	imshow("contours", capturedColorImage);
}

int main(void)
{
	VideoCapture cap(0);
	Mat a;

	while (1)
	{
		cap >> originalImage;

		/* filter for detect skin color */
		blur(originalImage, filteredImage, Size(3, 3));
		cvtColor(filteredImage, filteredImage, COLOR_BGR2HSV);
		inRange(filteredImage, Scalar(0, 50, 50), Scalar(30, 255, 255), filteredImage);

		imshow(filteredWindow, filteredImage);

		int key = waitKey(60);

		switch (key)
		{
		case 115:
			// s to save
			capture_image();
			break;
		case 108:
			// l to load
			load_image();
			break;
		case 99:
			// c to show result
			findcts();
			break;
		case 27:
			destroyAllWindows();
			return 0;
		default:
			break;
		}
	}

	destroyAllWindows();

	return 0;
}
