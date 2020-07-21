#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main() {

	Mat img = imread("./data/Ch15/lena.png", IMREAD_GRAYSCALE);

	Mat blur, deriv_X, deriv_Y, abs_deriv_X, abs_deriv_Y, result;

	GaussianBlur(img, blur, Size(5, 5), 5, 5, BORDER_DEFAULT);

	Sobel(blur, deriv_X, CV_16S, 1, 0, 3);

	convertScaleAbs(deriv_X, abs_deriv_X);

	Sobel(blur, deriv_Y, CV_16S, 0, 1, 3);

	convertScaleAbs(deriv_Y, abs_deriv_Y);

	addWeighted(abs_deriv_X, 0.5, abs_deriv_Y, 0.5, 0, result);

	imshow("img", img);
	imshow("blur", blur);
	imshow("Sobel_X", abs_deriv_X);
	imshow("Sobel_Y", abs_deriv_Y);
	imshow("result", result);
	waitKey();

	return 0;
}