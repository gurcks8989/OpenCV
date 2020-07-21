// g++ $(pkg-config --cflags --libs opencv) -std=c++11 histograme_show.cpp -o histograme_show

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//void show_hist(Mat& img, char* wname);
Mat drawHistogram(Mat src);

int main() {
	Mat img = imread("./data/Ch9/Moon.png", IMREAD_GRAYSCALE);
	Mat dst, img_histo, dst_histo;
	if (img.empty())
		return -1;

	img_histo = drawHistogram(img);
	equalizeHist(img, dst);
	dst_histo = drawHistogram(dst);

	imshow("org", img);
	imshow("org_histo", img_histo);
	imshow("eqlaized", dst);
	imshow("eqla_histo", dst_histo);

	waitKey(0);
	return 0;
}
Mat drawHistogram(Mat src) {

	Mat hist, histImage;
	// establish the number of bins
	int i, hist_w, hist_h, bin_w, histSize;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	hist_w = 512;
	hist_h = 400;
	histSize = 16;
	bin_w = cvRound((double)hist_w / histSize);
	// compute the histograms
	// &src: input image, 1: #of src image, 0: #of channels numerated from 0 ~ channels()-1, Mat(): optional mask
	// hist: output histogram, 1: histogram dimension, &histSize: array of histogram size,  &histRange: array of histogram¡¯s boundaries 
	calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange);
	//draw the histogram
	histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
	// normalize the result to [0, histImage.rows]
	// hist: input Mat, hist: output Mat, 0: lower range boundary of range normalization, histImage.rows: upper range boundary
	// NORM_MINMAX: normalization type, -1: when negative, the ouput array has the same type as src, Mat(): optional mask normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	for (i = 1; i < histSize; i++) {
		rectangle(histImage, Point(bin_w * i, hist_h), Point(bin_w * i, hist_h - cvRound(hist.at<float>(i))), Scalar(0, 0, 0), 2, 8, 0);
	}
	return histImage;
}
/*
void show_hist(Mat& img, char* wname) {
	Mat hist;
	float ranges[] = { 0, 255 };
	const float* histRange = { ranges };
	const int histSize = 16 ;
	bool uniform = true;
	bool accum = false;
	const int channels[] = { 0 };
	//const Mat img2 = img.clone();
	// compute histogram
	calcHist(&img, 1, channels, Mat(), hist, 1, &histSize, &histRange, uniform, accum);

	double maxVal = 0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);

	int scale = 2;
	Mat histImg = Mat::zeros(256 * scale, 256 * scale, CV_8UC3);

	for (int v = 0; v < 256; v++) {
		float binVal = hist.at<float>(v);
		int  num = cvRound(binVal * 255.0 / maxVal);

		rectangle(histImg, Point(v * scale, num * scale),
			Point((v + 1) * scale - 1, 0),
			//			Scalar(0, 200, 100), CV_FILLED);
			Scalar(0, 200, 100), FILLED);
	}

	flip(histImg, histImg, 0);

	imshow(wname, histImg);
}
*/