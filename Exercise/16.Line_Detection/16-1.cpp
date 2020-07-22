#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;


int main() {

	string link = "./data/Ch16/downtown_road.mp4";

	Mat frame, edge, result;

	vector<Vec4i> lines;

	VideoCapture source(link);

	if (!source.isOpened()) {
		cerr << "Can not founding the video!" << endl;
		return -1;
	}

	while (true) {

		source >> result;

		if (result.empty()) {
			cerr << "video Done!" << endl;
			break;
		}

		cvtColor(result, frame, COLOR_BGR2GRAY);

		//GaussianBlur(frame, frame, Size(3, 3), 10, 10);

		Mat roi(frame, Rect(Point(0, frame.rows / 2 + 30), Point(frame.cols, frame.rows -40 )));

		Canny(roi, edge,  80, 100, 3);

		//morphologyEx(edge, edge, MORPH_OPEN, Mat(), Point(-1, -1), 1);

		HoughLinesP(edge, lines, 1, CV_PI / 180, 50, 30, 1);

		for (int i = 0; i < lines.size(); i++) {
			Vec4i l = lines[i];
			line(result, Point(l[0], l[1] + frame.rows / 2 + 30), Point(l[2], l[3] + frame.rows / 2 + 30), Scalar(0, 0, 255), 3, 8);
		}


		imshow("frame", frame);
		imshow("result", result);
		imshow("edge", edge);
		imshow("roi", roi);

		waitKey(33);
	}
	return 0;
}