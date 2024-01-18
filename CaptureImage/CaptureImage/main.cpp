#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Mat img;
Point pointOld;
Point pointCurr;

void OnMouse(int event, int x, int y, int flags, void*)
{
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		pointOld = Point(x, y);	
		break;
	case EVENT_LBUTTONUP:
		pointCurr = Point(x, y);
		Mat newImg = img(Rect(pointOld.x, pointOld.y, pointCurr.x - pointOld.x, pointCurr.y - pointOld.y));
		if (pointOld.x == pointCurr.x && pointOld.y == pointCurr.y) { break; }
		imshow("captured_image", newImg);
		imwrite("captured_image.bmp", newImg);
		break;
	}
}

void CaptureImage() 
{
	img = imread("lenna.bmp");

	namedWindow("Lenna original");

	setMouseCallback("Lenna original", OnMouse);
	imshow("Lenna original", img);

	waitKey();
}

int main(void) 
{
	VideoCapture cap(0);

	while (1)
	{
		cap >> img;

		imshow("Video", img);

		setMouseCallback("Video", OnMouse);

		if (waitKey(1) == 27) { break; }
	}
}