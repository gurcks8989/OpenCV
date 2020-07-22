#include <iostream>
#include "opencv.hpp"
#include <math.h>

using namespace cv;
using namespace std;

int main(){

    VideoCapture capture("./data/Ch17/faces.mp4");
    String filenameFaceCascade = "./data/Ch17/haarcascade_frontalface_alt.xml";

    CascadeClassifier faceCascade;
    Mat image, gray;

    if (!faceCascade.load(filenameFaceCascade)){
        cerr << "error" << endl;
        return -1 ;
    }

    while (true){
        capture >> image;
        if (image.empty())
            break;

        cvtColor(image, gray, COLOR_BGR2GRAY);

        equalizeHist(gray, gray);

        // detect 
        vector<Rect> faces; 
        faceCascade.detectMultiScale(gray, faces, 1.3, 3, 0 | CASCADE_SCALE_IMAGE, Size(50, 50));

        for (int i = 0; i < faces.size(); i++) {
            rectangle(image, faces[i], Scalar(0, 0, 255)); // add rectangle to the image
        }
        imshow("image", image);

        imshow("gray", gray);

        waitKey(10);
    }
}
