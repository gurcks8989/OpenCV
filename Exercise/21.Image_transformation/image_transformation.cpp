#include "opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

struct MouseParams {
    Mat img;
    vector<Point2f> in, out;
};

int main() {

    string link = "./data/transformation.jpg";

    Mat frame = imread(link, IMREAD_COLOR);

    Mat result;

    resize(frame, frame, Size(600, 800));

    if (frame.empty()) {
        cerr << "can not found the image file" << endl; 
        return -1;
    }
    int x = frame.size().width;
    int y = frame.size().height;

    MouseParams mp;
    
    mp.out.push_back(Point2f(0, 0));
    mp.out.push_back(Point2f(210*2, 0));
    mp.out.push_back(Point2f(210*2, 297*2));
    mp.out.push_back(Point2f(0, 297*2));

    mp.in.push_back(Point2f(260, 90));
    mp.in.push_back(Point2f(569, 100));
    mp.in.push_back(Point2f(447, 534));
    mp.in.push_back(Point2f(23, 421));
    
    Mat homo_mat = getPerspectiveTransform(mp.in, mp.out);
    //homography 행령을 이용한 warp
    warpPerspective(frame, result, homo_mat, frame.size());

    imshow("output", result);

    waitKey();
    return 0;
}