#include <iostream>
#include "opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link = "./images/Fig06_color_bars.tif";
	Mat img = imread(link, IMREAD_COLOR);

	resize(img, img, Size(200, 200));
	imshow("org", img);

	vector<Mat> channel;
	split(img, channel);
	char wname[10] ;
	for (int i = 0; i < img.channels(); i++) {
		sprintf_s(wname, "Ch[%d]", i);
		imshow(wname, channel[i]);
	}
	Mat merged;
	channel[0] = channel[0] / 2;
	channel[1] = channel[1] / 2;
	merge(channel, merged);
	
	imshow("mrg", merged);
	waitKey(0);
	return 0;
}
