#include "opencv2/opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

int main() {
	Mat img = imread("./data/Ch12/Finger.png", IMREAD_GRAYSCALE);
	Mat dst;

	threshold(img, dst, 0, 255, THRESH_OTSU | THRESH_BINARY_INV);

	imshow("img", img);
	imshow("binary_inv", dst);

	waitKey();

	return 0;
}