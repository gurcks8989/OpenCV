#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link = "./data/Ch5/sun.jpg";

	Mat ori = imread(link);

	if (ori.empty()) {
		cerr << "read fail!" << endl;
		exit(-1);
	}

	Mat roi(ori, Rect(Point(50, 50), Point(400, 200)));

	roi = Scalar(255, 255, 0);

	imshow("original", ori);
	imshow("ROI", roi);
	waitKey(0);
	destroyAllWindows();

	return 0;
}
