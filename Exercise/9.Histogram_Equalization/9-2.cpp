#include <iostream>
#include "opencv.hpp"

using namespace std;
using namespace cv;
Mat drawHistogram(Mat src);
int main() {
	string link = "./data/Ch5/Apple.jpg";
	Mat img = imread(link, IMREAD_COLOR);
	Mat dst(img.rows, img.cols, CV_8UC1), origin_histo, equal_histo;

	Point a(160, 140), b(340, 370);
	vector<Mat> ch(3);

	split(img, ch);

	ch[2]; //R-chnnel
	origin_histo = drawHistogram(ch[2]);
	normalize(ch[2], dst);
	
	//equalizeHist(ch[2], dst);

	equal_histo = drawHistogram(dst);
	Mat red(dst(Rect(a, b)));
	
	imshow("origin", ch[2]);
	imshow("histograms", dst);
	imshow("red", red);
	imshow("origin_histo", origin_histo);
	imshow("equal_histo", equal_histo);

	waitKey();
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
	histSize = 8;
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