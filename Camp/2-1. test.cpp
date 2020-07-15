#include <iostream>
#include "opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link = "./images/lena.png";
	Mat img = imread(link, IMREAD_COLOR);

	//resize(img, img, Size(200, 200));
	//imshow("org", img);

	Mat roi(img, Rect(0, 0, 300, 300));
	Mat roi2_t(img, Rect(100, 0, 400, 400));
	Mat roi2 = roi2_t.clone();
	Mat dst, dst2, dst3;
	roi /= 2;

	vector<Mat> ch;
	split(roi2, ch);
	ch[2] = 0;
	merge(ch, roi2);

	cvtColor(img, dst, COLOR_RGB2BGR);

	cvtColor(dst, dst2, COLOR_BGR2GRAY);
	cvtColor(dst2, dst2, COLOR_GRAY2BGR);

	line(dst, Point(50, 100), Point(400, 500), Scalar(255, 255, 0), 3);
	rectangle(dst2, Rect(50, 50, 60, 60), Scalar(255, 0, 255), 1.0, 8, 0) ;
	/*
	void rectangle(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)

	void rectangle(Mat& img, Rect rec, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
	Thickness : negative value (CV_FILLED) means a filled rectangle

	void line(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)

	void circle(Mat& img, Point center, int radius, const Scalar& color, int thickness=1, int lineType=8, int shift=0)

	
	*/

	cvtColor(dst, dst2, COLOR_BGR2GRAY);

	imshow("roi", roi);
	imshow("roi2", roi2);
	imshow("img", img);
	imshow("dst", dst);
	imshow("dst2", dst2);



	waitKey(0);
	return 0;
}
