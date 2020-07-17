#include "opencv2/opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

int main() {

	string link = "./data/Ch13/background.mp4";

	VideoCapture video(link);
	
	Mat frame, background;

	video >> background;

	cvtColor(background, background, COLOR_BGR2GRAY);

	threshold(background, background, 200, 255, THRESH_OTSU | THRESH_BINARY_INV);

	imshow("background", background);

	while (true) {
		if (!video.isOpened())
			return -1;

		video >> frame;


		cvtColor(frame, frame, COLOR_BGR2GRAY);

		threshold(frame, frame, 200, 255, THRESH_OTSU | THRESH_BINARY_INV);

		subtract(frame, background, frame);

		morphologyEx(frame, frame, MORPH_OPEN, Mat(), Point(-1, -1), 2);

		imshow("video", frame);

		waitKey(10);

	}

	return 0;
}