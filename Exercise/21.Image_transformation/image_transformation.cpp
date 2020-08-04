#include "opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

struct MouseParams {
    Mat img;
    vector<Point2f> in, out;
};

int main() {

    //string link = "./data/image_transformation/transformation.jpg";
    string link = "./data/image_transformation/transformation_1.jpg";

    Mat frame = imread(link, IMREAD_COLOR);

    Mat gray, edge, kennel, result;

    resize(frame, frame, Size(600, 800));

    if (frame.empty()) {
        cerr << "can not found the image file" << endl;
        return -1;
    }
    int x = frame.size().width;
    int y = frame.size().height;

    cvtColor(frame, gray, COLOR_BGR2GRAY);

    GaussianBlur(gray, gray, Size(3, 3), 1);

    Canny(gray, edge, 8, 70, 3);

    kennel = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));

    morphologyEx(edge, edge, MORPH_CLOSE, kennel, Point(-1, -1), 3);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(edge.clone(), contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

    vector<Point2f> approx;
    result = frame.clone();

    int max = 0;
    int index = 0;
    for (size_t i = 0; i < contours.size(); i++) {
        if (max < arcLength(Mat(contours[i]), true)) {
            max = arcLength(Mat(contours[i]), true);
            index = i;
        }
    }
    approxPolyDP(Mat(contours[index]), approx, max * 0.02, true);

    MouseParams mp;

    //A4 size
    mp.out.push_back(Point2f(0, 0));
    mp.out.push_back(Point2f(210 * 2, 0));
    mp.out.push_back(Point2f(210 * 2, 297 * 2));
    mp.out.push_back(Point2f(0, 297 * 2));

    for (int i = 0; i < approx.size(); i++) {

        cout << "approx_" << i << approx[i] << endl;
        mp.in.push_back(approx[i]);
    }

    Mat homo_mat = getPerspectiveTransform(mp.in, mp.out);
    //homography 행령을 이용한 warp
    warpPerspective(frame, result, homo_mat, frame.size());

    imshow("input", frame);
    imshow("edge", edge);
    imshow("output", result);

    waitKey();

    return 0;
}