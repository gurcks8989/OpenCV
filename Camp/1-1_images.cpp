#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv; 
using namespace std;

int main(int argc, const char* argv[]) {
	string link = "./images/lena.png";
	Mat gray_image = imread(link, 0);
	Mat color_image = imread(link);
	if (gray_image.empty() || color_image.empty()) {
		cerr << "read fail!" << endl;
		exit(-1); 
	} 

	imshow("lena_color", color_image);
	imshow("lena_gray", gray_image);
	waitKey(0);
	destroyAllWindows();

	return 0; 
}
	