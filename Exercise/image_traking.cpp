#include <iostream>
#include <opencv.hpp>
#define img_size 5

using namespace cv;
using namespace std;

void setLabel(Mat& image, string str, Point* P);

string prof[] = { "prof.Kim", "prof.Ko", "prof.Lee", "prof.Seong", "prof.Yong" };


int main() {

	vector<Mat> img(img_size);
	VideoCapture video("./data/image_founding/video_2.mp4");
	Mat frame, gray, edge, HSV;
	vector<Mat>descriptors(img_size+1);
	Ptr<ORB> orbF = ORB::create(1000);
	vector<vector<KeyPoint>> keypoints(img_size+1);
	BFMatcher matcher(NORM_HAMMING);
	vector<Mat> face(img_size);
	face[0] = imread("./data/image_founding/face/Kim_Hojoon.jpg", IMREAD_COLOR);
	face[1] = imread("./data/image_founding/face/img_not_found.png", IMREAD_COLOR);
	face[2] = imread("./data/image_founding/face/Lee_Jongwon.jpg", IMREAD_COLOR);
	face[3] = imread("./data/image_founding/face/Sung_Keumyoung.jpg", IMREAD_COLOR);
	face[4] = imread("./data/image_founding/face/Yong_Whanki.jpg", IMREAD_COLOR);
	float nndr;
	vector <Point2f> in(4);

	if (!video.isOpened()) {
		cout << "cannot upload video!" << endl;
		return -1;
	}
	Size s(face[0].size() / 10);
	for (int i = 0; i < img_size; i++) {

		img[i] = imread("./data/image_founding/img/" + prof[i] + ".jpg", IMREAD_COLOR);
		if (img[i].empty() || face[i].empty()) {
			cout << "error!! img[" + to_string(i) + "] lost" << endl;
			return -1;
		}
		resize(img[i], img[i], img[i].size() / 10);
		resize(face[i], face[i], s);
		orbF->detectAndCompute(img[i], noArray(), keypoints[i], descriptors[i]);
	}
	
	in[0] = Point2f(0, 0);
	in[1] = Point2f(face[0].cols, 0);
	in[2] = Point2f(face[0].cols, face[0].rows);
	in[3] = Point2f(0, face[0].rows);

	while (true) {

		video >> frame;

		if (frame.empty()) {
			cout << "cannot find frame!!" << endl;
			break;
		}

		resize(frame, frame, frame.size() / 2);

		Mat main_descriptor;
		vector<KeyPoint> main_keypoint;
		vector<vector< DMatch >> goodMatches(img_size+1);
		int max = 0, sum = 0, index = img_size;
		vector <Point2f> out(4);

		orbF->detectAndCompute(frame, noArray(), main_keypoint, main_descriptor);

		for (int k = 0; k < img_size; k++) {
			vector< vector< DMatch> > matches;
			matcher.knnMatch(main_descriptor, descriptors[k], matches, 2);
			nndr = 0.65f;
			for (int i = 0; i < matches.size(); i++) {
				if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance) {
					goodMatches[k].push_back(matches[i][0]);
				}
			}
			size_t sizes = goodMatches[k].size();
			sum += sizes;
			if (max < sizes) {
				max = sizes;
				index = k;
			}
		}
		//cout << index << endl;
		

		float x = 0;
		if (index != img_size && max >= 10) {
			for (int i = 0; i < max; i++)	x += main_keypoint[goodMatches[index][i].queryIdx].pt.x;
			x /= max;
		}
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		cvtColor(frame, HSV, COLOR_BGR2HSV);
		vector<Mat>ch(3);
		split(HSV, ch);
		Mat result = frame.clone();
		for (int i = 0; i < 3; i++)	imshow("ch" + to_string(i), ch[i]);
		
		if (x + 270 < gray.cols) {
			Rect R(Point(0, gray.rows / 6), Point(gray.cols, gray.rows / 3 + 50));
			Mat roi(ch[1], R);
			//medianBlur(roi, roi, 1);
			imshow("2", roi);

			threshold(roi, roi, 36, 255, THRESH_BINARY);

			Mat kennel = getStructuringElement(MORPH_RECT, Size(3, 3));
			morphologyEx(roi, roi, MORPH_OPEN, kennel, Point(-1, -1), 3);
			morphologyEx(roi, roi, MORPH_CLOSE, kennel, Point(-1, -1), 1);
			imshow("3", roi);
			Rect R2(Point(x + 110, 10), Point(x + 270, roi.rows - 10));

			Mat roi2(roi, R2);

			imshow("roi", roi2);

			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			findContours(roi2.clone(), contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);// CHAIN_APPROX_NONE);// CHAIN_APPROX_SIMPLE);
			 
			//contour를 근사화한다.
			vector<Point2f> approx;

			for (size_t i = 0; i < contours.size(); i++) {
				double leng = arcLength(contours[i], true);
				approxPolyDP(Mat(contours[i]), approx, leng * 0.07, true);
				//if (x + 150 < approx[1].x && approx[1].x < x + 300) {
				line(result, Point(x + 110, 0), Point(x + 110, result.rows), Scalar(0, 255, 0));
				line(result, Point(x + 270, 0), Point(x + 270, result.rows), Scalar(0, 255, 0));
				
				//drawContours(result, contours, i, Scalar(0, 0, 255), 2);
				if (fabs(contourArea(Mat(approx))) < 1200 && fabs(contourArea(Mat(approx))) > 300 && leng < 150 && 90 < leng && approx.size() == 4) {// && x + 110 < approx[2].x && approx[2].x < x + 270) {  //면적이 일정크기 이상이어야 한다
					Point2f P = R2.tl() + R.tl();
					//cout << approx.size() << endl;
					int min = 1000;
					int j = -1;
					for (size_t k = 0; k < approx.size(); k++) {
						approx[k] += P;
						if (min > (approx[k].x + approx[k].y) / 2) {
							min = (approx[k].x + approx[k].y) / 2;
							j = k;
						}
					}
					if (j == 1)	for (size_t k = 0; k < approx.size() - 1; k++) 	swap(approx[k], approx[k + 1]);

					for (size_t j = 0; j < approx.size() - 1; j++)line(result, approx[j], approx[j + 1], Scalar(255, 0, 0), 2);
					line(result, approx[0], approx[3], Scalar(255, 0, 0), 2);
					//line(result, approx[0] + P, approx[1] + P, Scalar(255, 0, 0), 2);
					line(result, approx[0], approx[0], Scalar(0, 0, 255), 2);
					line(result, approx[1], approx[1], Scalar(0, 255, 255), 2);
					line(result, approx[2], approx[2], Scalar(255, 255, 255), 2);
					line(result, approx[3], approx[3], Scalar(0, 255, 0), 2);

					if (approx[3].x + 80 < result.cols){
						vector<double>a(2);

						out[0] = approx[3] + Point2f(30, 0);
						out[1] = out[0] + Point2f(50, 0);
						out[3] = out[0] + Point2f(0, 70);

						a[0] = (approx[3].y - approx[0].y) / (approx[3].x - approx[0].x);
						if (a[0] != 0)		out[1] += Point2f(0, a[0] * 50);
						if ((approx[1].x - approx[0].x) != 0) {
							a[1] = (approx[1].y - approx[0].y) / (approx[1].x - approx[0].x);
							out[3] += Point2f(70 / a[1], 0);
						}
						out[2] = Point2f(out[3].x + 50, out[1].y + 70);
						//cout << a[0] << "\t\t" << a[1] << endl;
					}
				}
			}

		}
		if (index == img_size || max < 10) {
			imshow("result", frame);
		}
		else {
			if (out[0] != Point2f(0, 0)) {
				Mat homo_mat = getPerspectiveTransform(in, out);
				Mat dst;
				//homography 행령을 이용한 warp
				warpPerspective(face[index], dst, homo_mat, frame.size());

				for (int y = 0; y < dst.rows; y++) {
					for (int x = 0; x < dst.cols; x++) {
						if (Vec3b(0, 0, 0) != dst.at<Vec3b>(y, x))
							result.at<Vec3b>(y, x) = dst.at<Vec3b>(y, x);
					}
				}
			}
			imshow("result", result);
		}
		int key = waitKey(20);
		if (key == 27)
			break;
	}
	return 0;
}

void setLabel(Mat& image, string str, Point* P)
{
	int fontface = FONT_HERSHEY_TRIPLEX;
	double scale = 1;
	int thickness = 1;
	int baseline = 0;

	Size text = getTextSize(str, fontface, scale, thickness, &baseline);

	putText(image, str, *P, fontface, scale, Scalar(0, 255, 0), thickness, 8);

	*P += Point(15, text.height * 2);
}

