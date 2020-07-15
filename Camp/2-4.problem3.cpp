#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link = "./images/bird.png";
	Mat img = imread(link, IMREAD_COLOR);

	//resize(img, img, Size(200, 200));
	//imshow("org", img);

	vector<Mat> ch(8);
	vector<Point> P(8);
	for (int i = 0; i < 8; i++) {
		findContours(img, ch[1], 3, 4, P[1]);
	}
	inRange(img, )

		ch[0] = img.clone();
	ch[1] =
		ch[2] =
		ch[3] =
		ch[4] = 0;
	ch[5] =
		ch[6] =
		ch[7] = 0;

	imshow("roi", roi);
	imshow("roi2", roi2);
	imshow("img", img);
	waitKey(0);
	return 0;
}
