#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link = "./data/Ch4/lena.png";

	Mat color_image = imread(link);
	Mat gray_image = imread(link, 0);

	if (color_image.empty() || gray_image.empty()) {
		cerr << "read fail!" << endl;
		exit(-1);
	}

	imshow("lena_color", color_image);
	imshow("lena_gray", gray_image);
	waitKey(0);
	destroyAllWindows();

	return 0;
}
