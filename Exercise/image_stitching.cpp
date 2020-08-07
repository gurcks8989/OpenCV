#include <opencv.hpp>
#include <iostream>
#include <string>
#define image_size 4

using namespace std;
using namespace cv;

struct PP {
	vector<Point2f> in, out;
};

int main() {

	vector<Mat>img(image_size);
	vector<Mat>descriptors(image_size);
	vector<Mat>dst(image_size);
	vector<Mat>H(image_size);
	Mat result;
	//ORB Ŭ���� ��ü�� �����Ͽ� orbF ����Ʈ �����Ϳ� ������.
	/*
	feature ������ Ptr<Feature2D>Ÿ������ ����Ǿ�����,
	�����δ� ORB Ŭ���� ��ü�� ����Ű�� ����.
	�׷��Ƿ� orbF->detect()���·� �Լ��� ȣ���ϸ�
	������ ORB �˰��� ���� Ŭ������ detect()�Լ��� �����.
	ORB Ư¡���� keypoints�� �����.
	*/
	Ptr<ORB> orbF = ORB::create(1000);
	vector<vector<KeyPoint>> keypoints(image_size);
	vector< vector< DMatch> > matches;
	vector< DMatch > goodMatches;
	BFMatcher matcher(NORM_HAMMING);
	Mat imgMatches;
	vector <Point2f> obj;
	vector <Point2f> scene;
	vector <Point2f> objP(4);
	vector <Point2f> sceneP(4);
	int k = 0;
	float nndr;

	/*
	int max_x[4] = { 0 };
	int max_y[4] = { 0 };
	int min_x[4] = { 10000, 10000, 10000, 10000 };
	int min_y[4] = { 10000, 10000, 10000, 10000 };

	int max_x_1[4] = { 0 };
	int max_y_1[4] = { 0 };
	int min_x_1[4] = { 10000, 10000, 10000, 10000 };
	int min_y_1[4] = { 10000, 10000, 10000, 10000 };
	*/
	for (int i = 0; i < image_size; i++) {
		img[i] = imread("./data/image_stitching/backup/20200803_" + to_string(i) + ".jpg", IMREAD_COLOR);

		if (img[i].empty()) {
			cout << "cannot find image file" << endl;
			return -1;
		}

		resize(img[i], img[i], img[i].size() / 6);
		orbF->detectAndCompute(img[i], noArray(), keypoints[i], descriptors[i]);
		/*
		//ORB Ű����Ʈ ����ڸ� ����Ͽ� desc��Ŀ� ������.

		���� keypoints�� ���޵� Ư¡������ ����ڸ� ����� �� ������,
		�ش� Ư¡���� ���ŵ�.
		���� �� Ư¡���� ����ڴ� ��� descriptors ��Ŀ� �����.
		���� Ư¡�� ������ ����, �� ������ ����ϴ� ����� �˰��� ���� �ڵ����� ������.
		*/
	}

	objP[0] = Point2f(0, 0);
	objP[1] = Point2f(img[k].cols, 0);
	objP[2] = Point2f(img[k].cols, img[k].rows);
	objP[3] = Point2f(0, img[k].rows);

	//while(k < image_size-1){ // 3�� ����

	//KNN Matching
	matcher.knnMatch(descriptors[k], descriptors[k+1], matches, 2);
	nndr = 0.6f;
	for (int i = 0; i < matches.size(); i++) {
		if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance) {
			goodMatches.push_back(matches[i][0]);
		}
	}//Draw matching
	drawMatches(img[k], keypoints[k], img[k+1], keypoints[k+1], goodMatches, imgMatches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	if (goodMatches.size() < 4) { cout << "Matching failed" << endl; return 0; }

	//Find perspective transform
	/*
	1. queryIdx
	: ������ �Ǵ� descriptor �� keypoint�� index�̴�.
	matches[0]�� desA[0]�� �������� ��� ������
	matches[0].queryIdx = 0 �̴�.
	2. trainIdx
	: desA[0]�� ��Ī�� image B, des�� index�� �ش��Ѵ�.
	3. distance
:	: desA[0]�� ��Ī�� desB�� des ������ �Ÿ�( = ���絵 )���̴�.
   */
	for (int i = 0; i < goodMatches.size(); i++) {
		obj.push_back(keypoints[k][goodMatches[i].queryIdx].pt);
		scene.push_back(keypoints[k+1][goodMatches[i].trainIdx].pt);
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
	*/
	H[k] = findHomography(obj, scene, RANSAC);

	obj.clear();
	scene.clear();

	perspectiveTransform(objP, sceneP, H[k]);
	//sceneP �ƿ�ǲ

	float min_x = 0, min_y = 0;

	for (int i = 0; i < 4; i++) {
		if (min_x > sceneP[i].x)
			min_x = sceneP[i].x; 
		if (min_y > sceneP[i].y)
			min_y = sceneP[i].y;
	}

	Point2f P(min_x, min_y);
	for (int i = 0; i < 4; i++) sceneP[i] -= P;
	/*
	cout << "obj" << obj << endl;
	cout << "scene" << scene << endl;
	cout << "max_x  :  " << max_x[0] << "\t\tmax_y  :  " << max_y[0] << endl;
	cout << "min_x  :  " << min_x[0] << "\t\tmin_y  :  " << min_y[0] << endl;
	cout << "max_x  :  " << max_x_1[0] << "\t\tmax_y  :  " << max_y_1[0] << endl;
	cout << "min_x  :  " << min_x_1[0] << "\t\tmin_y  :  " << min_y_1[0] << endl;
	cout << "obj" << objP << endl;
	cout << "scene" << sceneP << endl;
	//cout << "min_x_2  :  " << min_x_2 << "\t\tmin_y_2  :  " << min_y_2 << endl;
	*/

	imshow("imgMatches", imgMatches);
	
	dst[k+1] = Mat::zeros(img[k+1].rows * 2, img[k+1].cols * 3, img[k+1].type());
	imshow("dst11", dst[k+1]);
	Mat homo_mat = getPerspectiveTransform(objP, sceneP);
	
	//homography ����� �̿��� warp
	warpPerspective(img[k], dst[k], homo_mat, Size(img[k].cols * 3, img[k].rows * 2));
	
	for (int y = 0; y < img[1].rows; y++){
		for (int x = 0; x < img[1].cols; x++){
			int newX = x -P.x;
			int newY = y -P.y;
			if (newX >= 0 && newX < dst[1].cols && newY >= 0 && newY < dst[1].rows)
				dst[1].at<Vec3b>(newY, newX) = img[1].at<Vec3b>(y, x);
		}
	}
	//result = dst[0] | dst[1];
	Mat gyo = dst[0] & dst[1];
	imshow("inside_" + to_string(k), gyo);
	for (int y = 0; y < gyo.rows; y++) {
		for (int x = 0; x < gyo.cols; x++) {
			if (Vec3b(0, 0, 0) == gyo.at<Vec3b>(y, x))
				gyo.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
			else
				gyo.at<Vec3b>(y, x) = Vec3b(0, 0, 0);
		}
	}
	imshow("gyo_dot", gyo);
	// �� ���ֱ� 
	morphologyEx(gyo, gyo, MORPH_OPEN, Mat(), Point(-1, -1), 1);

	Mat dst_1 = (gyo & dst[k]) | (gyo & dst[k + 1]);
	//result = dst[k + 1] - gyo + dst[k];
	addWeighted(dst[k], 0.5, dst[k + 1], 0.5, 1, result);
	imshow("result_before", result);
	addWeighted(result, 1, dst_1, 0.5, 1, result);
	//for (int i = 0; i < image_size; i++) {
		//drawKeypoints(img[i], keypoints[i], dst[i], Scalar::all(-1), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	//line(img[i], Point(obj[i].x, obj[i].y), Point(obj[i+1].x, obj[i+1].y), Scalar(0, 0, 255), 2, 8);
		//imshow("img" + to_string(i), img[i]);
		//imshow("dst" + to_string(i), dst[i]);
	//}
	//Rect R1(Point(min_x_1[0],                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      min_y_1[0]), Point(max_x_1[0], max_y_1[0]));
	//rectangle(dst[i], R1, Scalar(255, 0, 0), 1);
	imshow("gyo", gyo);
	imshow("dst0", dst[k]);
	imshow("dst1", dst[k + 1]);
	imshow("dst_1", dst_1);
	imshow("result", result);
	imshow("img1", img[1]);

	waitKey(0);

	return 0;

}