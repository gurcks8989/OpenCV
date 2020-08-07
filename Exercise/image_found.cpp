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
	Mat frame, gray, edge;
	vector<Mat>descriptors(img_size);
	Ptr<ORB> orbF = ORB::create(1000);
	vector<vector<KeyPoint>> keypoints(img_size);
	BFMatcher matcher(NORM_HAMMING);
	Mat imgMatches;
	float nndr;

	/*
	vector <Point2f>obj; //::iterator obj;
	vector <Point2f> scene;
	vector <Point2f> objP(4);
	vector <Point2f> weight(4);
	vector <Point2f> sceneP(4);
	*/
	if (!video.isOpened()) {
		cout << "cannot upload video!" << endl;
		return -1;
	}

	for (int i = 0; i < img_size; i++) {

		img[i] = imread("./data/image_founding/img/" + prof[i] + ".jpg", IMREAD_COLOR);
		if (img[i].empty()) {
			cout << "error!! img[" + to_string(i) + "] lost" << endl;
			return -1;
		}
		resize(img[i], img[i], img[i].size()/10);
		orbF->detectAndCompute(img[i], noArray(), keypoints[i], descriptors[i]);
	}
	/*
	sceneP[0] = Point2f(0, 0);
	sceneP[1] = Point2f(img[0].cols, 0);
	sceneP[2] = Point2f(img[0].cols, img[0].rows);
	sceneP[3] = Point2f(0, img[0].rows);
	*/
	while (true) {
		video >> frame;

		if (frame.empty()) {
			cout << "cannot find frame!!" << endl;
			break;
		}

		resize(frame, frame, frame.size()/2);

		Mat main_descriptor;
		vector<KeyPoint> main_keypoint;
		vector<vector< DMatch >> goodMatches(img_size);
		int max = 0, sum = 0, index = img_size;

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

		if (index == img_size || max < 10){
			Mat zero = Mat::zeros(Size(frame.cols + img[0].cols, frame.rows), frame.type());
			Mat M(zero, Rect(Point(0, 0), frame.size()));
			frame.copyTo(M);
			imshow("imgMatches", zero);
			/*
			for (int y = 0; y < img[1].rows; y++) {
				for (int x = 0; x < img[1].cols; x++) {
					zero.at<Vec3b>(y, x) = frame.at<Vec3b>(y, x);
				}
			}
			*/
		}
		else {
	/*
	1. queryIdx
	: 기준이 되는 descriptor 및 keypoint의 index이다.
	matches[0]는 desA[0]를 기준으로 삼기 때문에
	matches[0].queryIdx = 0 이다.
	2. trainIdx
	: desA[0]과 매칭된 image B, des의 index에 해당한다.
	3. distance
:	: desA[0]와 매칭된 desB의 des 사이의 거리( = 유사도 )값이다.
   
			for (int i = 0; i < goodMatches[index].size(); i++) {
				obj.push_back(main_keypoint[goodMatches[index][i].queryIdx].pt);
				scene.push_back(keypoints[index][goodMatches[index][i].trainIdx].pt);
			}
			/*
			if (max_x[0] < obj[i].x)
				max_x[0] = obj[i].x;
			if (min_x[0] > obj[i].x)
				min_x[0] = obj[i].x;
			if (max_y[0] < obj[i].y)
				max_y[0] = obj[i].y;
			if (min_y[0] > obj[i].y)
				min_y[0] = obj[i].y;

			if (max_x_1[0] < scene[i].x)
				max_x_1[0] = scene[i].x;
			if (min_x_1[0] > scene[i].x)
				min_x_1[0] = scene[i].x;
			if (max_y_1[0] < scene[i].y)
				max_y_1[0] = scene[i].y;
			if (min_y_1[0] > scene[i].y)
				min_y_1[0] = scene[i].y;
		}
		Rect L(Point(min_x[0], min_y[0]), Point(max_x[0], max_y[0]));

		rectangle(imgMatches, L, Scalar(255, 0, 0), 1);

		Rect R(Point(min_x_1[0] + img[0].cols, min_y_1[0]), Point(max_x_1[0] + +img[0].cols, max_y_1[0]));

		rectangle(imgMatches, R, Scalar(255, 0, 0), 1);

		Mat LRoi(img[0], L);
		Mat RRoi(img[1], R);
		
			Mat H = findHomography(obj, scene, RANSAC);

			obj.clear();
			scene.clear();

			perspectiveTransform(sceneP, objP, H);
			//sceneP 아웃풋
			//objP[0] += Point2f(2 * (objP[1].x - objP[0].x), 200);
			//objP[3] += Point2f(2 * (objP[2].x - objP[3].x), 200);
			//for (int i = 0; i < 4; i+=2) objP[i] += Point2f(2*(objP[i+1].x - objP[i].x),200);
			for (int i = 0; i < 4; i++) objP[i] += Point2f(img[0].cols, 200);

			cout << objP << endl;
			//cout << frame.size() << endl;
			Mat homo_mat = getPerspectiveTransform(sceneP, objP);

			Mat dst;

			//homography 행령을 이용한 warp
			warpPerspective(img[index], dst, homo_mat, Size(img[index].cols * 3, img[index].rows * 2));
			imshow("dst", dst);
			*/
			Point* P = &Point(frame.cols + img[0].cols / 15, img[0].rows * 1.2);
			int percent = 10000 * max / sum;
			//Draw matching
			drawMatches(frame, main_keypoint, img[index], keypoints[index], goodMatches[index], imgMatches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
			//putText
			setLabel(imgMatches, "This office is ", P);
			setLabel(imgMatches, prof[index] + "'s", P);
			setLabel(imgMatches, "office", P);
			setLabel(imgMatches, "with " + to_string(percent/100) + "." + to_string(percent%100) + "%.", P);
			imshow("imgMatches", imgMatches);
		}
		int key = waitKey(33);
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

	*P += Point(15, text.height*2);
}

