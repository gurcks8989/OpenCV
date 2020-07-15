// g++ $(pkg-config --cflags --libs opencv) -std=c++11 video_read.cpp -o video_read
#include "opencv2/opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

int main(){
    Mat frame;
	VideoCapture cap("./images/Highway.mp4");

	if (!cap.isOpened()){
		cerr << "file open fail" << endl;
		exit(-1);
	}
	while (1) {
		cap >> frame;
		if (frame.empty()){
			cerr << "empty frame" << endl;
			break;
		}
		imshow("vid", frame);
		waitKey(33);
	}
}
  