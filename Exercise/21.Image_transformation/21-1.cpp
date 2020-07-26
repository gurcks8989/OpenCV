#include "opencv.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

struct MouseParams {
    Mat in_img, out_img;
    vector<Point2f> in, out;
    bool run ;
};

bool transformation = false;

static void onMouse(int event, int x, int y, int, void* param) {
    MouseParams* mp = (MouseParams*)param;
    Mat result = mp->out_img;
    if (event == EVENT_LBUTTONDOWN){ // left button 
        //왼쪽 상단부터 시계방향으로 입력
        mp->out.push_back(Point2f(x, y));

        if (mp->out.size() == 4) {
            mp->run = true;
            return;
        }
        else {
            result = mp->out_img;
            for (size_t i = 0; i < mp->out.size(); i++)
            {
                circle(result, mp->out[i], 3, Scalar(0, 0, 255), 5);
            }
            imshow("output", result);
        }
    }
    //클릭 reset
    if(event == EVENT_RBUTTONDOWN) {
        mp->out.clear();
        imshow("output", result);
        mp->run = false;
        transformation = false;
    }
}

int main() {
    /*
    Mat imput = imread("book.PNG");
    imshow("input", imput);
    MouseParams mp;
    mp.out.push_back(Point2f(0, 0));
    mp.out.push_back(Point2f(200, 0));
    mp.out.push_back(Point2f(200, 200));
    mp.out.push_back(Point2f(0, 200));
    mp.img = imput;
    setMouseCallback("input", onMouse, (void*)&mp);
    waitKey();
    return 0;
*/

	string link_display = "./data/Ch21/contest.mp4";
	string link_background = "./data/Ch21/timesquare.mp4";

    VideoCapture display(link_display);
    VideoCapture video(link_background);

	Mat frame, background;

    MouseParams mp;

	if (!video.isOpened() || !display.isOpened())
		return -1;

    video >> background;
    display >> frame;

    int x = frame.size().width;
    int y = frame.size().height;

    mp.in.push_back(Point2f(0, 0));
    mp.in.push_back(Point2f(x, 0));
    mp.in.push_back(Point2f(x, y));
    mp.in.push_back(Point2f(0, y));

    mp.out_img = background;

    mp.run = false;

    imshow("output", background);

    Point trapezoid[1][4];
    int npt[] = { 4 };


	while (true) {
        //cerr << "run is " << mp.run << "\n transgormation is " << transformation << endl;

        if (mp.run) {
            transformation = true;
            mp.run = false;
        }

        setMouseCallback("output", onMouse, (void*)&mp);

        video >> background;
        display >> frame;

		if (frame.empty() || background.empty())
			break;

        if (transformation) {
            Mat piece;

            for (int i = 0; i < 4; i++)
                trapezoid[0][i] = Point(mp.out[i].x, mp.out[i].y);

            //네 쌍의 매칭쌍으로 부터 homography 행렬계산
            const Point* ppt[1] = { trapezoid[0] };
            fillPoly(background, ppt, npt, 1, Scalar(0, 0, 0), 8);

            Mat homo_mat = getPerspectiveTransform(mp.in, mp.out);
            //homography 행령을 이용한 warp
            warpPerspective(frame, piece, homo_mat, background.size());

            imshow("piece", piece);
            background += piece;
        }
        imshow("output", background);

		waitKey(33);

	}
	return 0;
}