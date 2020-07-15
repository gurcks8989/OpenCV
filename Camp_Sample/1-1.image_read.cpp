// g++ $(pkg-config --cflags --libs opencv) -std=c++11 image_read.cpp -o image_read
#include "opencv2/opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

int main(){
    Mat gray_image;
    Mat color_image;
    gray_image = imread("./images/lena.png", 0 );
    color_image = imread("./images/lena.png");
    if (gray_image.empty() || color_image.empty()) {
        cout << "file read error" << endl;
        exit(-1);
    }
    imshow("gray image", gray_image);
    imshow("color image", color_image);
    waitKey(0);
    destroyAllWindows();
}
