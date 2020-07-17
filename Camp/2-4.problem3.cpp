#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link = "./images/face_senior.jpg";
	Mat img = imread(link, IMREAD_COLOR);
	Mat hsv, dst, dst2, face_range, Head, kennel, merged;

	//resize(img, img, Size(200, 200));
	//imshow("org", img);

	GaussianBlur(img, img, Size(3, 3), 1.5, 1.5);

	cvtColor(img, hsv, COLOR_BGR2HSV);

	inRange(hsv, Scalar(0, 0, 0), Scalar(20, 255, 255), dst);
	inRange(hsv, Scalar(160, 0, 0), Scalar(180, 255, 255), dst2);
	face_range = dst + dst2;
	kennel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	morphologyEx(face_range, face_range, MORPH_OPEN, kennel, Point(-1, -1), 4);
	//erode(dst3, dst3, Mat(), Point(-1, -1), 3);

	/*
	Canny(img, canny, 80, 130);

	dilate(canny, canny, Mat(), Point(-1, -1), 1);

	imshow("Canny", canny);
	*/

	vector<Mat> ch(4), face(4);
	split(hsv, ch);
	Mat cd;
	int i = 16;

	Head = ch[2].clone();
	ch[2] -= face_range;
	bitwise_and(face_range, Head, face[2]);
	bilateralFilter(face[2], cd, i, i*2, i/2);
	bitwise_or(ch[2], cd, ch[2]) ;


	Head = ch[1].clone();
	ch[1] -= face_range;
	bitwise_and(face_range, Head, face[1]);
	face[1] += 20;
	bitwise_or(ch[1], face[1], ch[1]);

	merge(ch, merged);

	cvtColor(merged, merged, COLOR_HSV2BGR);

	imshow("original", img);
	imshow("face range", face_range);
	imshow("after blur", merged);
	

	waitKey(0);
	return 0;
}
