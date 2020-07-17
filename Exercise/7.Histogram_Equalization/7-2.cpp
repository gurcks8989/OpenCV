#include <iostream>
#include "opencv.hpp"

using namespace std;
using namespace cv;

int main() {
	string link = "./data/Ch9/Apple.jpg";
	Mat img = imread(link, IMREAD_COLOR);
	Mat dst(img.rows, img.cols, CV_8UC1);

	const int pt_x = 160, pt_y = 140, pt2_x = 340, pt2_y = 370;
	vector<Mat> ch(3);

	split(img, ch);

	ch[2]; //R-chnnel
	equalizeHist(ch[2], dst);

	Mat red = ch[2].clone();
	
	red.copyTo(dst(Rect(pt_x, pt_y, pt2_x, pt2_y)));

	imshow("histograms", dst);

	waitKey();
	return 0;
}