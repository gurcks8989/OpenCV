#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link_apple = "./data/Ch5/apple.jpg";
	string link_lena = "./data/Ch5/lena.png";

	Mat apple = imread(link_apple);
	Mat lena = imread(link_lena);
	Mat dst, dst2;

	if (apple.empty() || lena.empty()) {
		cerr << "read fail!" << endl;
		exit(-1);
	}

	subtract(apple, lena, dst);
	absdiff(apple, lena, dst2);

	imshow("apple", apple);
	imshow("lena", lena);
	imshow("sub", dst);
	imshow("abs", dst2);

	waitKey(0);
	destroyAllWindows();

	return 0;
}
