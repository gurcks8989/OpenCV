// g++ $(pkg-config --cflags --libs opencv) -std=c++11 10-2.coin_count.cpp -o 10-1.coin_count
#include "opencv2/opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

Mat apply_gamma_transform(Mat& gray, double gamma);
void get_logTable(uchar lut[]);
void prepare_lut(unsigned char lut[], float fGamma, float C);

int main() {
	//Mat img = imread("./images/coin_B16.jpg");
	Mat img = imread("./images/coin_B10.jpg");
	Mat src_gray, src, gray, dst, bin, open, dist, dist_norm, sure_bg, sure_fg, morph;

	float width = 500;
	float height = width * ((double)img.rows / (double)img.cols);

	resize(img, src, Size(width, height));
	imshow("input", src);

	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	GaussianBlur(src_gray, gray, Size(11, 11), 1.5, 1.5);
	imshow("gray", gray);


	unsigned char lut1[256], lut2[256], log_lut[256];

	Mat T = apply_gamma_transform(gray, 2.0);
	Mat element5x5 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Mat element3x3 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));

	imshow("org gray", gray);
	imshow("gamma=T2", T);

	threshold(T, bin, 0, 255, THRESH_OTSU | THRESH_BINARY);
	imshow("OTSU", bin);
	medianBlur(bin, bin, 15);
	erode(bin, bin, element3x3);
	imshow("binary image (OTSU + median)", bin);

	morphologyEx(bin, open, MORPH_OPEN, element3x3, Point(-1, -1), 2);
	imshow("opening", open);

	dilate(open, sure_bg, element3x3, Point(-1, -1), 3);
	imshow("sure_bg (close)", sure_bg);


	Mat dist_8U;
	//	distanceTransform(open, dist, CV_DIST_L2, 5);
	distanceTransform(open, dist, DIST_L2, 5);
	normalize(dist, dist_norm, 0, 1, NORM_MINMAX);
	dist_norm = dist_norm * 255;
	dist_norm.convertTo(dist_8U, CV_8U);
	imshow("normalize", dist_8U);

	double minv, maxv;
	minMaxIdx(dist_8U, &minv, &maxv);
	threshold(dist_8U, sure_fg, maxv * 0.6, 255, THRESH_BINARY);
	dilate(sure_fg, sure_fg, element5x5, Point(-1, -1), 2);
	imshow("dist_thresh", sure_fg);


	//*** special post processing
	imshow("final fg", sure_fg);

	Mat unknown;
	subtract(sure_bg, sure_fg, unknown);
	imshow("unknown", unknown);

	// Find total markers
	vector<vector<Point> > contours;
	findContours(sure_fg.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	// Create the marker image for the watershed algorithm
	Mat markers = Mat::zeros(sure_bg.size(), CV_8UC1);
	// Draw the foreground markers
	for (size_t i = 0; i < contours.size(); i++) {
		drawContours(markers, contours, i, Scalar(static_cast<int>(i) + 1), -1);
		markers = markers + 1; // make bg == 1
		int coin_num = contours.size();

		// Draw the background marker
		bitwise_not(unknown, unknown); // make unknown region a zero
		bitwise_and(unknown, markers, markers);

		Mat colormap;
		applyColorMap(markers * (255.0 / (coin_num + 1)), colormap, COLORMAP_JET);
		imshow("colormapped markers", colormap);

		markers.convertTo(markers, CV_32S);
		//imshow("Markers", markers * 10000);

		Mat imgResult(src);
		// Perform the watershed algorithm
		watershed(imgResult, markers);

		//imshow("watershed result", markers*(80000.0/(float)coin_num));

		// Generate random colors
		vector<Vec3b> colors;
		for (size_t i = 0; i < contours.size() + 1; i++)
		{
			int b = theRNG().uniform(0, 256);
			int g = theRNG().uniform(0, 256);
			int r = theRNG().uniform(0, 256);
			colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
		}
		// Create the result image
		Mat dst = Mat::zeros(markers.size(), CV_8UC3);
		// Fill labeled objects with random colors
		for (int i = 0; i < markers.rows; i++)
		{
			for (int j = 0; j < markers.cols; j++)
			{
				int index = markers.at<int>(i, j);
				if (index == 255) {    // background
					dst.at<Vec3b>(i, j) = Vec3b(255, 255, 0);
				}
				else if (index == -1) { // border
					dst.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
				}
				else if (index > 0 && index <= static_cast<int>(contours.size()))
				{
					dst.at<Vec3b>(i, j) = colors[index - 1];
				}
			}
		}
		// Visualize the final image
		imshow("result", dst);

		waitKey(0);
		return coin_num;
	}
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