#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat apply_gamma_transform(Mat& gray, double gamma);
void get_logTable(uchar lut[]);
void prepare_lut(unsigned char lut[], float fGamma, float C);

int main() {
    Mat img = imread("./data/Ch11/lena.png", IMREAD_COLOR);
    Mat rgb, hsv;
    int gamma = 0.5;

	if (img.empty())
		return 0;

	float width = 500;
	float height = width * ((double)img.rows / (double)img.cols);

	resize(img, img, Size(width, height));

	cvtColor(img, rgb, COLOR_BGR2RGB);
	cvtColor(img, hsv, COLOR_BGR2HSV);

	vector<Mat> ch(3);
	imshow("h", hsv);
	
	hsv = apply_gamma_transform(hsv, gamma);



	split(hsv, ch);

	Mat Intensity = ch[2].clone();

	rgb = apply_gamma_transform(rgb, gamma);

	split(rgb, ch);

	imshow("intensity", Intensity);
	imshow("red", ch[0]);
	imshow("green", ch[1]);
	imshow("blue", ch[2]);

	waitKey(0);
	destroyAllWindows();
}

Mat apply_gamma_transform(Mat & gray, double gamma) {
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