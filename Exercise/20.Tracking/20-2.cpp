#include <iostream>
#include "opencv.hpp"
#include <math.h>

using namespace cv;
using namespace std;

struct CallbackParam {
    Mat frame;
    Point pt1, pt2;
    Rect roi;
    bool drag;
    bool updated;
};
/*
struct feature {
    Point2f pt; 
    int val;
};
*/

bool initialization = false;
void DrawTrackingPoints(Rect R, vector<Point2f>& points, Mat& image);

void onMouse(int event, int x, int y, int flags, void* param) {
    CallbackParam* p = (CallbackParam*)param;
    if (p->frame.size().width < x)
        x = p->frame.size().width;
    if (p->frame.size().height < y)
        y = p->frame.size().height;
    
    if (event == EVENT_LBUTTONDOWN) {
        p->pt1.x = x;
        p->pt1.y = y;
        p->pt2 = p->pt1;
        p->drag = true;
    }
    if (event == EVENT_LBUTTONUP) {             //마우스 놓을때
        int w = x - p->pt1.x;
        int h = y - p->pt1.y;
        p->roi.x = p->pt1.x;
        p->roi.y = p->pt1.y;
        p->roi.width = w;
        p->roi.height = h;
        p->drag = false;
        if (w >= 10 && h >= 10) {
            p->updated = true;
        }
    }
    if (p->drag && event == EVENT_MOUSEMOVE) {
        if (p->pt2.x != x || p->pt2.y != y) {
            Mat img = p->frame.clone();
            p->pt2.x = x;
            p->pt2.y = y;
            rectangle(img, p->pt1, p->pt2, Scalar(0, 255, 0), 1); 
            imshow("dstImage", img);
        }
        
    }
}


int main() {

    VideoCapture cap("./data/Ch14/background.mp4");

    CallbackParam param;

    double fps = cap.get(CAP_PROP_FPS);
    Mat currImage, prevImage;
    Mat frame, dstImage;
    Rect m_rc;
    double qualityLevel = 0.01;
    double minDistance = 10;
    int blockSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;
    int maxCorners = 500;
    TermCriteria criteria = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 10, 0.01); 
    Size winSize(11, 11);
    vector<Point2f> prevPoints;
    vector<Point2f> currPoints;
    vector<Point2f> boundPoints;
    int delay = 1000 / fps; 
    int nframe = 0;
    param.updated = false;
    param.drag = false;

    while (1) {

        if (param.updated) {
            /*
            Point p, p2;
            if (param.pt1.x <= param.pt2.x) {
                p.x = param.pt1.x;
                p2.x = param.pt2.x;
            }
            else {
                p.x = param.pt2.x;
                p2.x = param.pt1.x;
            }
            if (param.pt1.y <= param.pt2.y) {
                p.y = param.pt1.y;
                p2.y = param.pt2.y;
            }
            else {
                p.y = param.pt2.y;
                p2.y = param.pt1.y;
            }
            */

            m_rc = param.roi;
            param.updated = false;
            initialization = true;
        }
        cap >> frame;

        if (frame.empty())
            break;

        frame.copyTo(dstImage);

        param.frame = dstImage;
        setMouseCallback("dstImage", onMouse, &param);

        Mat pre_roi(prevImage, m_rc);

        /// Copy the source image
        cvtColor(dstImage, currImage, COLOR_BGR2GRAY);
        GaussianBlur(currImage, currImage, Size(5, 5), 0.5);

        Mat curr_roi(currImage, m_rc);

        //feature detection
        if (initialization) {
            cerr << "Point1(" << m_rc.tl() << "), Point2(" << m_rc.br() << ")" << endl;
            goodFeaturesToTrack(pre_roi, prevPoints, maxCorners, qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, k);
            cornerSubPix(pre_roi, prevPoints, winSize, Size(-1, -1), criteria);
            DrawTrackingPoints(m_rc, prevPoints, dstImage);
            initialization = false;
        }
        if (prevPoints.size() > 0) {
            vector<Mat> prevPyr, currPyr;
            Mat status, err;
            buildOpticalFlowPyramid(pre_roi, prevPyr, winSize, 3, true);
            buildOpticalFlowPyramid(curr_roi, currPyr, winSize, 3, true);
            calcOpticalFlowPyrLK(prevPyr, currPyr, prevPoints, currPoints, status, err, winSize); //delete invalid correspondinig points
            for (int i = 0; i < prevPoints.size(); i++) {
                if (!status.at<uchar>(i)) {
                    prevPoints.erase(prevPoints.begin() + i);
                    currPoints.erase(currPoints.begin() + i);
                }
            }
            DrawTrackingPoints(m_rc, currPoints, dstImage);
            prevPoints = currPoints;
        }
        imshow("dstImage", dstImage);
        
        currImage.copyTo(prevImage);
        int ch = waitKey(33);
        if (ch == 27)               // 27 == ESC key
            break;
        //if (ch == 32)               //  SPACE Key (pause)
            //initialization = true;
    }
    return 0;
}

void DrawTrackingPoints(Rect R, vector<Point2f>& points, Mat& image) {
    // Draw corners detected

    for (int i = 0; i < points.size(); i++) {
        int x = cvRound(points[i].x)+R.tl().x;
        int y = cvRound(points[i].y)+R.tl().y;
        circle(image, Point(x, y), 3, Scalar(255, 0, 0), 2);
    }
}
/*
        }
        if (param.updated) {
            Rect rc = param.roi;
            Mat mask = Mat::zeros(rc.height, rc.width, CV_8U);
            ellipse(mask, Point(rc.width / 2, rc.height / 2), Size(rc.width / 2, rc.height / 2), 0, 0, 360, 255); Mat roi(hsv, rc);
            calcHist(&roi, 1, channels, mask, m_model3d, 3, hist_sizes, ranges);
            m_rc = rc;
            param.updated = false;
            tracking = true;
        }
        cap >> frame;

        if (frame.empty())
            break;


        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // image processing 
        if (tracking) {
            //histogram backprojection
            calcBackProject(&hsv, 1, channels, m_model3d, m_backproj, ranges);
            //tracking
            CamShift(m_backproj, m_rc, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 20, 1));
            //meanShift(m_backproj, m_rc, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1)); 
            rectangle(frame, m_rc, Scalar(0, 0, 255), 3);
        }
        // image display
        imshow("Tracker", frame);
        // user input
        char ch = waitKey(33);
        if (ch == 27) break; // ESC Key (exit) 
        else if (ch == 32) { // SPACE Key (pause)
            while ((ch = waitKey(33)) != 32 && ch != 27);
            if (ch == 27)
                break;
        }
    }
    return 0;
}

*/