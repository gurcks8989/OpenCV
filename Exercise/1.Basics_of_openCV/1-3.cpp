#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link = "./data/Ch4/background.mp4";

	VideoCapture Faster(link);
	VideoCapture Slower(link);

	if (!Faster.isOpened() || !Slower.isOpened())
	{
		cerr << "Can't open video file!" << endl;
		return -1;
	}

	while (true) {

		Mat faster, slower;

		Faster >> faster;
		Slower >> slower;

		if (Faster.read(faster))
			Faster.read(faster);

		if (!Faster.read(faster) || !Slower.read(slower)) // if not success, break loop
				// read() decodes and captures the next frame.
		{
			cout << "\n Cannot read the video file. \n";
			break;
		}


		imshow("faster", faster);

		//press ESC on keyboard to exit
		char c = (char)waitKey(100);

		imshow("slower", slower);
		if (c == 27)
			break;
	}
	Faster.release();
	Slower.release();

	destroyAllWindows();

	return 0;
}
