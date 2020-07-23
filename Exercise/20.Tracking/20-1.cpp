#include <iostream>
#include "opencv.hpp"
#include <math.h>

using namespace cv;
using namespace std;

struct CallbackParam {
    Mat image;
    Point pt1, pt2;
    Rect roi;
    bool drag;
    bool updated;
};

int main() {

    VideoCapture capture("./data/Ch17/faces.mp4");
    String filenameFaceCascade = "./data/Ch17/haarcascade_frontalface_alt.xml";

    CascadeClassifier faceCascade;
    Mat image, gray;

    if (!capture.isOpened()) {
        cerr << "can not found the video file!" << endl;
        return -1;
    }

    if (!faceCascade.load(filenameFaceCascade)) {
        cerr << "error" << endl;
        return -1;
    }
    
    capture >> image;

    cvtColor(image, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);

    // detect 
    vector<Rect> faces;
    faceCascade.detectMultiScale(gray, faces, 1.3, 3, 0 | CASCADE_SCALE_IMAGE, Size(50, 50));

    for (int i = 0; i < faces.size(); i++) 
        rectangle(image, faces[i], Scalar(0, 0, 255)); // add rectangle to the image


    CallbackParam param;
    Mat m_backproj, m_model3d, hsv;
    Rect m_rc = faces[0];
    float hrange[] = { 0,180 };
    float vrange[] = { 0,255 };
    const float* ranges[] = { hrange, vrange, vrange }; // hue, saturation, brightness
    int channels[] = { 0, 1, 2 };
    int hist_sizes = 20;

    // click and drag on image to set ROI
    imshow("Tracker", image);

    /*
    param.image = image;
    param.drag = false;
    param.updated = false;
    //setMouseCallback("Tracker", onMouse, &param);
    */
    while (true) {

        cvtColor(image, hsv, COLOR_BGR2HSV);

        Mat mask = Mat::zeros(m_rc.height, m_rc.width, CV_8U);
        ellipse(mask, Point(m_rc.width / 2, m_rc.height / 2), Size(m_rc.width / 2, m_rc.height / 2), 0, 0, 360, Scalar(255));
        Mat roi(hsv, m_rc);
        calcHist(&roi, 1, channels, mask, m_model3d, 1, &hist_sizes, ranges);

        capture >> image;
        if (image.empty())
            break;

        //histogram backprojection
        calcBackProject(&hsv, 1, channels, m_model3d, m_backproj, ranges);
        //tracking
        meanShift(m_backproj, m_rc, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
        //CamShift(m_backproj, m_rc, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 20, 1));
        rectangle(image, m_rc, Scalar(0, 0, 255));

        /*
        // detect 
        vector<Rect> faces;
        faceCascade.detectMultiScale(gray, faces, 1.3, 3, 0 | CASCADE_SCALE_IMAGE, Size(50, 50));

        for (int i = 0; i < faces.size(); i++) {
            rectangle(image, faces[i], Scalar(0, 0, 255)); // add rectangle to the image
        }
        */
        imshow("image", image);
        //imshow("roi", roi);

        //imshow("hsv", hsv);

        if (waitKey(33) == 27) 
            break;
    }
}
