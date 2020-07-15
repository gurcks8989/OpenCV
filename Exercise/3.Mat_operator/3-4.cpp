#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link_lena = "./data/Ch5/lena.png";

	Mat lena = imread(link_lena, 0);
	Mat dst, dst2;

	if (lena.empty()) {
		cerr << "read fail!" << endl;
		exit(-1);
	}

	imshow("org", lena);

	adaptiveThreshold(lena, dst, 200, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 19, 0);
	imshow("MEAN_C", dst);
	adaptiveThreshold(lena, dst, 200, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 19, 0);
	imshow("GAUSSIAN_C", dst);

	waitKey(0);

	return 0;
}
