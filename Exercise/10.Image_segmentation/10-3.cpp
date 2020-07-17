#include "opencv2/opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;


Mat apply_gamma_transform(Mat& gray, double gamma);
void get_logTable(uchar lut[]);
void prepare_lut(unsigned char lut[], float fGamma, float C);

int main() {
	Mat img = imread("./data/Ch12/adaptive.png", IMREAD_GRAYSCALE);
	Mat dst;

	img = apply_gamma_transform(img, 0.01);
	threshold(img, dst, 0, 255, THRESH_OTSU | THRESH_BINARY_INV);

	imshow("img", img);
	imshow("binary_inv", dst);

	waitKey();

	return 0;
}


Mat apply_gamma_transform(Mat& gray, double gamma) {
	unsigned char lut[256];

	prepare_lut(lut, gamma, 1);
	Mat table(1, 256, CV_8UC1, lut);
	Mat T = gray.clone();
	LUT(gray, table, T);
	return T;
}

void get_logTable(uchar lut[])
{
	double rmax = 255;

	cout << "LOG transform Table " << endl;
	double C = 255.0 / log10(rmax + 1);
	for (int i = 0; i < 256; i++) {
		float v = C * std::log10((float)i + 1.0);
		lut[i] = saturate_cast<uchar>(v);
	}
}

void prepare_lut(unsigned char lut[], float fGamma, float C)
{
	cout << "Gamma Transform Table" << endl;
	float max = 0;
	for (int i = 0; i < 256; i++) {
		float v = pow((float)(i / 255.0), fGamma) * 255.0f * C;
		lut[i] = saturate_cast<uchar>(v);
		cout << i << ':' << v << ':' << (int)lut[i] << endl;
	}

}