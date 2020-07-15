#include <iostream>
#include "opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {
	string link = "./images/lena.png";
	Mat img = imread(link, IMREAD_COLOR) ;
	Mat dst, dst2, dst3;

	if (img.empty()) {
		cerr << "read fail!" << endl;
		exit(-1);
	}

	cvtColor(img, dst, COLOR_BGR2GRAY);
	cvtColor(img, dst2, COLOR_BGR2HSV);
	cvtColor(img, dst3, COLOR_BGR2YUV);

	imshow("img", img);
	imshow("GRAY", dst);
	imshow("HSV", dst2);
	imshow("YUV", dst3);

	waitKey(0);
	destroyAllWindows();

	return 0;
}
