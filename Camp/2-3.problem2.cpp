#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

void setLabel(Mat& image, string str, vector<Point> contour);


int main(int argc, const char* argv[]) {

	string link = "./images/chair.PNG";
	Mat img = imread(link, IMREAD_COLOR);
	Mat gray, result, dst, kennel, hsv, canny;

	if (img.empty()) {
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	GaussianBlur(img, img, Size(7, 7), 1.5, 1.5);
	Canny(img, canny, 80, 130);

	dilate(canny, canny, Mat(), Point(-1, -1), 1);

	imshow("Canny", canny);

	//contour를 찾는다.
	int cnt = 0;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(canny.clone(), contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

	//contour를 근사화한다.
	vector<Point2f> approx;
	result = img.clone();

	for (size_t i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

		if (fabs(contourArea(Mat(approx))) > 100)  //면적이 일정크기 이상이어야 한다.
		{
			Scalar color(rand() % 256, rand() % 256, rand() % 256);
			drawContours(result, contours, i, color, 2);
			int size = approx.size();
				//도형을 판정한다.
			if (size == 3)
				setLabel(result, "T", contours[i]); //삼각형

			//이하는 해당 꼭지점을 가진 convex라면 찾는 도형
			else if (size == 4) {
				double length = sqrt(pow(approx[0].x - approx[1].x, 2) + pow(approx[0].y - approx[1].y, 2));
				double AVG_length = arcLength(Mat(contours[i]), true) / (double)size;
				double error_lange = 5;
				if (length - error_lange <= AVG_length && AVG_length < length + error_lange)
					setLabel(result, "SR", contours[i]); //정사각형
				else
					setLabel(result, "pR", contours[i]); //사각형
			}
		}
	}

	imshow("input", img);
	imshow("result", result);
		
	waitKey(0);

	return 0;
}

void setLabel(Mat& image, string str, vector<Point> contour)
{
	int fontface = FONT_HERSHEY_SIMPLEX;
	double scale = 0.5;
	int thickness = 1;
	int baseline = 0;

	Size text = getTextSize(str, fontface, scale, thickness, &baseline);

	vector<Point2f> approx;
	approxPolyDP(Mat(contour), approx, arcLength(Mat(contour), true) * 0.02, true);
	double X = 0, Y = 0; 

	for (int i = 0; i < approx.size(); i++) {
		X += approx[i].x;
		Y += approx[i].y;
	}
	Point pt((X / approx.size()), (Y / approx.size()));

	putText(image, str, pt, fontface, scale, CV_RGB(0, 0, 0), thickness, 8);
}

