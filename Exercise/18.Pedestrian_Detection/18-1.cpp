#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main() {
	
	string link = "./data/Ch18/pedestrian.avi";

	VideoCapture source(link);

	Mat img, color;

	if (!source.isOpened())
		return -1;

	Ptr<BackgroundSubtractor> pBackSub;
	pBackSub = createBackgroundSubtractorMOG2();


	while (true) {
		source >> img;

		if (img.empty())
			break;

		color = img.clone();
		//cvtColor(img, gray, COLOR_BGR2GRAY);

		pBackSub->apply(img, img);

		morphologyEx(img, img, MORPH_OPEN, Mat(), Point(-1, -1), 2);

		//dilate(frame, frame, kennel,  Point(-1, -1), 3);


		vector<vector<Point> > contours;
		findContours(img.clone(), contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

		vector<Rect> boundRect(contours.size());
		vector<Point2f>centers(contours.size());

		int count = 0;

		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(contours[i], centers, 3, true);
			if (fabs(contourArea(Mat(centers), true)) > 200)  //면적이 일정크기 이상이어야 한다.
			{
				boundRect[i] = boundingRect(centers);
				rectangle(color, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 2);
				count++;
			}
		}

		string source = "The number of moving objects counting : " + to_string(count);
		putText(color, source, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1, 8);

		imshow("video orig", color);

		imshow("video gray", img);

		waitKey(33);
	}
	return 0;
}