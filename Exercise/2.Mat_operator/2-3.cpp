#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link = "./data/Ch5/Night.mp4";

	VideoCapture Night(link);

	if (!Night.isOpened()) {
		cerr << "Can't open the video file!" << endl;
		return -1;
	}

	while (true) {

		Mat input, dst;

		Night >> input;

		if (Night.read(input))
			Night.read(input);

		if (!Night.read(input)) // if not success, break loop
				// read() decodes and captures the next frame.
		{
			cout << "\n Cannot read the video file. \n";
			break;
		}

		convertScaleAbs(input, dst, 5, 10);

		imshow("Frame", dst);
	
		if (waitKey(20) == 27)
			break;
	}
	Night.release();

	destroyAllWindows();

	return 0;
}
