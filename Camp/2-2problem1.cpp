// g++ $(pkg-config --cflags --libs opencv) -std=c++11 video_read.cpp -o video_read
#include "opencv2/opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

int main() {
    Mat frame, gray_images;
    VideoCapture cap("./images/images/Highway-Surveillance.mp4");
    int key = -1;
    int change = 0;

    if (!cap.isOpened()) {
        cerr << "file open fail" << endl;
        exit(-1);
    }


    for (;;) {
        cap >> frame;

        //int key = waitKey(10); // waitKey(ms), ms값 = 동영상 재생속도

        if (change == 0 && key == -1) {
            imshow("vid", frame);
        }

        if (change == 1 && key == -1) {
            cvtColor(frame, gray_images, COLOR_BGR2GRAY);
            imshow("vid", gray_images);
        }

        key = waitKey(10);

        // 32 : space bar
        if (key == 32) {
            key = waitKey(0);

            if (key == 32) {

                if (change == 1) {
                    imshow("vid", gray_images);
                }
                else if (change == 0) {
                    imshow("vid", frame);
                }
            }

        }
        else


            if (key == '0') { // 0
                change = 1;
                key = -1;
            }
        if (key == '1') { // 1
            change = 0;
            key = -1;
        }
        if (key == 27) {
            break; // esc 누르면 종료
        }

        //imshow("vid", frame);

        //waitKey(33);

    }
}