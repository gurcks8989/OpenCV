// g++ $(pkg-config --cflags --libs opencv) -std=c++11 image_gamma.cpp -o image_gamma
#include "opencv2/opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

Mat apply_gamma_transform(Mat& gray, double gamma);
void get_logTable(uchar lut[]);
void prepare_lut(unsigned char lut[], float fGamma, float C);
void GammaCorrection(int, void*);
void GammaCorrection2(int, void*);

static Mat img, img2, T, T2;
static int g;

int main() {
	unsigned char lut1[256], lut2[256], log_lut[256];
	img = imread("./data/Ch8/reverse_light_1.jpg", IMREAD_GRAYSCALE);

	if (img.empty())
		return -1;

	float width = 500;
	float height = width * ((double)img.rows / (double)img.cols);

	resize(img, img, Size(width, height));

	img2 = img.clone();
	img2 += 50;

	namedWindow("gamma1");

	createTrackbar("Gamma1", "gamma1", &g, 200, GammaCorrection);

	waitKey(0);
	destroyAllWindows();

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
void GammaCorrection(int, void*) {
	T = apply_gamma_transform(img, (double)g * 0.1);
	T2 = apply_gamma_transform(img2, (double)g * 0.1);
	imshow("Gamma1", T);
	imshow("Gamma2", T2);
}