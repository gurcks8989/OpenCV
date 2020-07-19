#include "opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main() {

	Mat img = imread("./images/word_thresh.png", 0);
	Mat binary, kennel, dst, dst2, openning, closing;


	if (img.empty())
		return -1;
		/*
	threshold(img, binary, 180, 200, THRESH_BINARY);
	
	kennel = getStructuringElement(MORPH_RECT, Size(3, 3));

	erode(binary, dst, kennel);
	dilate(dst, dst2, kennel);
	morphologyEx(binary, openning, MORPH_OPEN, kennel, Point(-1, -1), 0);
	morphologyEx(binary, closing, MORPH_CLOSE, kennel, Point(-1, -1), 0);

	imshow("word_thresh.png", img);
	imshow("binary", binary);
	imshow("erosion", dst);
	imshow("dilation", dst2);
	imshow("open", openning);
	imshow("close", closing);
	*/
	waitKey(0);

	return 0;

}
