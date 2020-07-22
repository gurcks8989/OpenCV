#include <iostream>
#include "opencv.hpp"

using namespace cv;
using namespace std;

void sum_pixel(Mat iimage, Rect R);

int main() {

	string link = "./data/Ch15/lena.png";

	Mat gray = imread(link, IMREAD_GRAYSCALE);

	Mat iimage, dst, binary = gray.clone();;
	integral(gray, iimage, CV_32S);  // 적분 영상 계산 interger  

	//eye_cascade = cv.CascadeClassifier('haarcascade_eye.xml')

	Rect R1(Point(100, 100), Point(200, 200));
	Rect R2(Point(150, 150), Point(170, 170));
	Rect R3(Point(200, 200), Point(210, 210));
	/*
	int blockSize = 21; // 이웃 크기
	//int threshold = 10;  // 화소를 (평균 - 경계값)과 비교
	//int64 time;

	int nl= binary.rows; 
    int nc= binary.cols; 
              
 
	//convertScaleAbs(iimage, dst);
    int halfSize= blockSize/2;
	for (int j = halfSize; j < nl - halfSize - 1; j++)  //포인터를 이용한 반복문
	{
		uchar* data = binary.ptr<uchar>(j);
		int* idata1 = iimage.ptr<int>(j - halfSize);
		int* idata2 = iimage.ptr<int>(j + halfSize + 1);

		for (int i = halfSize; i < nc - halfSize - 1; i++) {

			// 영역 계산(D-B-C+A)
			int sum = (idata2[i + halfSize + 1] - idata2[i - halfSize] -
				idata1[i + halfSize + 1] + idata1[i - halfSize]) / (blockSize * blockSize);
			/*
			// apply adaptive threshold
			if (data[i] < (sum - threshold))
				data[i] = 0;
			else
				data[i] = 255;
			
		}
	}

	*/

	//convertScaleAbs(iimage, iimage, CV_32S);

	sum_pixel(iimage, R1);
	sum_pixel(iimage, R2);
	sum_pixel(iimage, R3);


	imshow("integral", iimage);
	imshow("gray", gray);



	/*
	Mat ele1(sum, Rect(Point(100, 100), Point(200, 200)));
	Mat ele2(sum, Rect(Point(150, 150), Point(170, 170)));
	Mat ele3(sum, Rect(Point(200, 200), Point(210, 210)));

	//Mat dst, dst2, result;
	
	imshow("ele1", ele1);
	imshow("ele2", ele2);
	imshow("ele3", ele3);

	//addWeighted(gray, 0.25, ele1, 0.25, 0.5, dst);
	//addWeighted(ele2, 0.25, ele3, 0.25, 0.5, dst2);
	//result = dst + dst2;
	
	*/
	//imshow("result", result);

	waitKey();

	return 0;
}

void sum_pixel(Mat iimage, Rect R) {

	int* idata1 = iimage.ptr<int>(R.tl().y);
	int* idata2 = iimage.ptr<int>(R.br().y);

	int sum = (idata2[R.br().x] - idata2[R.tl().x] -
		idata1[R.br().x] + idata1[R.tl().x]);// / ((R1.br().x - R1.tl().x) * (R1.br().y - R1.tl().y));

}