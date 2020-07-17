#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
	Mat img = imread("./data/Ch10/saltnpepper.png", IMREAD_GRAYSCALE);
	Mat blurred, medianed;

	int i = 7;
	blur(img, blurred, Size(i, i));

	medianBlur(img, medianed, i);

	imshow("blur", blurred);
	imshow("median", medianed);


	waitKey();
	return 0;
}