#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link = "./data/Ch4/background.mp4";

	VideoCapture input(link);

	if (!input.isOpened())
	{
		cerr << "Can't open video file!" << endl;
		return -1;
	}

	while (true) {

		Mat frame;

		input >> frame;

		if (!input.read(frame)) // if not success, break loop
				// read() decodes and captures the next frame.
		{
			cout << "\n Cannot read the video file. \n";
			break;
		}

		imshow("Frame", frame);

		//press ESC on keyboard to exit
		char c = (char)waitKey(25);
		if (c == 27)
			break;
	}
	input.release();

	destroyAllWindows();

	return 0;
}
