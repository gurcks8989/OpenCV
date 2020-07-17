#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
	Mat img = imread("./data/Ch10/Moon.png", IMREAD_GRAYSCALE);
    Mat blurred, dst;
    blur(img, blurred, Size(7, 7));
    addWeighted(img, 2.0, blurred, -1.0, 0.0, dst);
    imshow("org", img);
    imshow("blurred", blurred);
    imshow("shapen", dst);

    waitKey(0);
}