#include "opencv2/opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

int main() {

	string link = "./data/Ch13/background.mp4";	

	VideoCapture video(link);

	Mat frame;
	Ptr<BackgroundSubtractor> pBackSub;
	pBackSub = createBackgroundSubtractorMOG2();
	
	while (true) {
		video >> frame;
		if (frame.empty())
			break;
		//update the background model
		pBackSub->apply(frame, frame);

		morphologyEx(frame, frame, MORPH_OPEN, Mat(), Point(-1, -1), 1);

		imshow("video", frame);

		waitKey(10);

	}

	return 0;
}