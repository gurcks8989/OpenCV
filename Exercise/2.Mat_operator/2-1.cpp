#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link = "./data/Ch5/lena.png";

	Mat img = imread(link);
	Mat dst;

	if (img.empty()) {
		cerr << "read fail!" << endl;
		exit(-1);
	}
	cvtColor(img, dst, COLOR_BGR2HSV);
	
	vector<Mat> ch(3);

	split(dst, ch);

	imshow("Original", img);
	imshow("H", ch[0]);
	imshow("L", ch[1]);
	imshow("S", ch[2]);

	waitKey(0);
	destroyAllWindows();

	return 0;
}
