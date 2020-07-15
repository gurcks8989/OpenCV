#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	string link = "./images/bird.png";
	Mat img = imread(link, IMREAD_COLOR);
	Mat gray, result;

	if (img.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	//�׷��̽����� �̹����� ��ȯ  
	cvtColor(img, gray, COLOR_BGR2GRAY);

	//����ȭ �̹����� ��ȯ
	Mat binary_image;
	threshold(gray, binary_image, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	/*
	//contour�� ã�´�.
	vector<vector<Point> > contours;
	findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

	//contour�� �ٻ�ȭ�Ѵ�.
	vector<Point2f> approx;
	result = img.clone();

	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.02, true);

		if (fabs(contourArea(Mat(approx))) > 100)  //������ ����ũ�� �̻��̾�� �Ѵ�. 
		{

			int size = approx.size();

			//Contour�� �ٻ�ȭ�� ������ �׸���.
			if (size % 2 == 0) {
				line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

				for (int k = 0; k < size - 1; k++)
					line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);

				for (int k = 0; k < size; k++)
					circle(img_result, approx[k], 3, Scalar(0, 0, 255));
			}
			else {
				line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

				for (int k = 0; k < size - 1; k++)
					line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);

				for (int k = 0; k < size; k++)
					circle(img_result, approx[k], 3, Scalar(0, 0, 255));
			}



			//������ �����Ѵ�.
			if (size == 3)
				setLabel(img_result, "triangle", contours[i]); //�ﰢ��

			//���ϴ� �ش� �������� ���� convex��� ã�� ����
			else if (size == 4 && isContourConvex(Mat(approx)))
				setLabel(img_result, "rectangle", contours[i]); //�簢��

			else if (size == 5 && isContourConvex(Mat(approx)))
				setLabel(img_result, "pentagon", contours[i]); //������

			else if (size == 6 && isContourConvex(Mat(approx)))
				setLabel(img_result, "hexagon", contours[i]);  //������

			else if (size == 10 && isContourConvex(Mat(approx)))
				setLabel(img_result, "decagon", contours[i]);    //�ʰ���

			//�� ���ǿ� �ش� �ȵǴ� ���� ã�Ƴ� ������ ������ ǥ��
			else setLabel(img_result, to_string(approx.size()), contours[i]);
		}

	}


	imshow("input", img_input);
	imshow("result", img_result);


	waitKey(0);

	//resize(img, img, Size(200, 200));
	//imshow("org", img);
	
	vector<Mat> ch(8);
	vector<Point> P(8);
	for (int i = 0; i < 8; i++) {
		findContours(img, ch[i], 3, 4, P[i]);
	}

	//inRange(img, );

	ch[0] = img.clone();
	ch[1] = 
	ch[2] =
	ch[3] =
	ch[4] = 0;
	ch[5] =
	ch[6] =
	ch[7] = 0;
	
	imshow("roi", roi);
	imshow("roi2", roi2);
	imshow("img", img);
	/**/
	imshow("gray", gray);
	imshow("binary", binary_image);


	waitKey(0);

	return 0;
}
