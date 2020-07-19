#include "opencv2/opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

int main() {

	string link = "./data/Ch13/background.mp4";

	VideoCapture video(link);
	
	if (!video.isOpened())
		return -1;

	Mat frame, background, kennel, result;

	video >> background;
	/*
	cvtColor(background, background, COLOR_BGR2GRAY);

	adaptiveThreshold(background, background, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 9, 10);
	*/
	//threshold(background, background, 0, 255, THRESH_OTSU | THRESH_BINARY_INV);

	imshow("background", background);

	while (true) {

		video >> frame;

		if (frame.empty())
			break;


		subtract(background, frame, frame);

		cvtColor(frame, frame, COLOR_BGR2GRAY);

		medianBlur(frame, frame, 9);

		//adaptiveThreshold(frame, frame, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 1);

		threshold(frame, frame, 40, 255, THRESH_BINARY);

		//subtract(frame, background, frame);

		kennel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));

		morphologyEx(frame, frame, MORPH_OPEN, kennel, Point(-1, -1), 1);

		dilate(frame, frame, kennel,  Point(-1, -1), 3);

		imshow("video orig", frame);

		vector<vector<Point> > contours;
		findContours(frame.clone(), contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

		vector<Rect> boundRect(contours.size());
		vector<Point2f>centers(contours.size());
		vector<float>radius(contours.size());

		cvtColor(frame, result, COLOR_GRAY2BGR);

		int count = 0;

		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(contours[i], centers, 3, true);
			if (fabs(contourArea(Mat(centers), true)) > 200)  //면적이 일정크기 이상이어야 한다.
			{
				boundRect[i] = boundingRect(centers);
				rectangle(result, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 2);
				count++;
			}
		}
			/*
			approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

			if (fabs(contourArea(Mat(approx))) > 200)  //면적이 일정크기 이상이어야 한다.
			{	//drawContours(result, contours, i, Scalar(0, 0, 255), 1);
				
				double X = 0, Y = 0;

				for (int i = 0; i < approx.size(); i++) {
					X += approx[i].x;
					Y += approx[i].y;
				}
				Point pt((X / approx.size()), (Y / approx.size()));

				
			}	*/

		string source = "The number of moving objects counting : " + to_string(count);
		putText(result, source, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1, 8);


		imshow("video", result);

		waitKey(10);

	}

	return -1;
}