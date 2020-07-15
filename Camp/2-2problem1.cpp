#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main() {
	VideoCapture capture("./images/Highway-Surveillance.mp4");
	Mat frame, grayImage;
	bool stop = false;

	for (; ; ) {
		if (!stop) {
			capture >> frame;
			if (!capture.read(frame)) {
				cerr << "can't reading the video" << endl;
				break;
			}
			//cvtColor를 사용하여 흑백으로 변경
			//imshow를 사용해 원본과 grayImage 출력

			char key = waitKey(30);
			if (key == 27)
				break;
		}

	}
}

	
