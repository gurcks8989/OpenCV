#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link = "./data/Ch4/lena.png";

	Mat img(400, 500, CV_8UC3, Scalar(0, 255, 255)) ;

	imshow("img", img);

	waitKey();

	return 0;
}
