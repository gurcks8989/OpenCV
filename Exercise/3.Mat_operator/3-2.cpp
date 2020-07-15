#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link_apple = "./data/Ch5/apple.jpg";
	string link_spine = "./data/Ch5/Fracture_spine.png";

	Mat apple = imread(link_apple);
	Mat img = imread(link_spine);
	Mat dst;

	if (apple.empty() || img.empty()) {
		cerr << "read fail!" << endl;
		exit(-1);
	}

	//addWeighted(apple, 0.7, img, 0.3, 0.0, dst);
	//add(apple, img, dst);
	dst = apple, img;
	
	imshow("apple", apple);
	imshow("img", img);
	imshow("add", dst);

	waitKey(0);
	destroyAllWindows();

	return 0;
}
