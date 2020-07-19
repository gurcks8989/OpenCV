#include "opencv2/opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

int main() {

	string link = "./data/Ch13/background.mp4";

	VideoCapture video(link);
	
	if (!video.isOpened())
		return -1;

	Mat frame, background;

	video >> background;

	cvtColor(background, background, COLOR_BGR2GRAY);

	adaptiveThreshold(background, background, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 5);

	//threshold(background, background, 0, 255, THRESH_OTSU | THRESH_BINARY_INV);

	imshow("background", background);

	while (true) {

		video >> frame;

		if (frame.empty())
			break;
		cvtColor(frame, frame, COLOR_BGR2GRAY);

		adaptiveThreshold(frame, frame, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 10);

		//threshold(frame, frame, 0, 255, THRESH_OTSU | THRESH_BINARY_INV);

		subtract(frame, background, frame);

		morphologyEx(frame, frame, MORPH_OPEN, Mat(), Point(-1, -1), 1);

		imshow("video", frame);

		waitKey(10);

	}

	return 0;
}