#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link_lena = "./data/Ch5/lena.png";

	Mat lena = imread(link_lena, 0);
	Mat dst;

	if (lena.empty()) {
		cerr << "read fail!" << endl;
		exit(-1);
	}

	imshow("org", lena);
	threshold(lena, dst, 128, 200, THRESH_BINARY);
	imshow("BINARY", dst);
	threshold(lena, dst, 128, 200, THRESH_BINARY_INV);
	imshow("INV", dst);
	threshold(lena, dst, 128, 200, THRESH_TRUNC);
	imshow("TRUNC", dst);
	threshold(lena, dst, 128, 200, THRESH_TOZERO);
	imshow("TOZERO", dst);

	waitKey(0);

	return 0;
}
