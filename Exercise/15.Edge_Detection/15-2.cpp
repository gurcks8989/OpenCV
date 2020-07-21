#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;


static const char* win_name = "Canny";
static int lowThreshold, highThreshold;

Mat frame, result;

void CannyThreshold(int, void*);

int main() {
	
	string link = "./data/Ch14/background.mp4";

	VideoCapture source(link);

	if (!source.isOpened()) {
		cerr << "Can not founding the video!" << endl;
		return -1;
	}

	while (true) {

		source >> frame;

		if (frame.empty()) {
			cerr << "video Done!" << endl;
			break;
		}

		result.create(frame.size(), frame.type());

		GaussianBlur(frame, frame, Size(3, 3), 10, 10);

		namedWindow(win_name, WINDOW_AUTOSIZE);

		createTrackbar("lowThres", win_name, &lowThreshold, 200, CannyThreshold);
		createTrackbar("highThres", win_name, &highThreshold, 200, CannyThreshold);
		
		waitKey(10);
	}
	return 0;
}

void CannyThreshold(int, void*) {
	Canny(frame, result, lowThreshold, highThreshold, 3);
	imshow(win_name, result);
}