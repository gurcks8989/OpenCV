#include "opencv.hpp"
#include <iostream> 
#include <fstream> 

using namespace cv;
using namespace std;
using namespace dnn;

int main() {

	string link = "./data/Ch19/Go_1.mp4";

	VideoCapture cap(link);

	Mat frame;

	if (!cap.isOpened()) {
		cerr << "can not found the video file!" << endl;
		return -1;
	}

	String modelConfiguration = "./data/Ch19/deep/yolov2.cfg";
	String modelBinary = "./data/Ch19/deep/yolov2.weights";

	Net net = readNetFromDarknet(modelConfiguration, modelBinary);

	vector<String> classNamesVec;
	string modelName = "./data/Ch19/deep/coco.names";
	ifstream classNamesFile(modelName.c_str());
	if (classNamesFile.is_open()) {
		string className = "";
		while (getline(classNamesFile, className))
			classNamesVec.push_back(className);
	}

	while (true) {

		cap >> frame; // get a new frame from camera/video or read image 

		if (frame.empty()) {
			waitKey();
			break;
		}

		if (frame.channels() == 4)
			cvtColor(frame, frame, COLOR_BGRA2BGR);

		//Convert Mat to batch of images
		Mat inputBlob = blobFromImage(frame, 1 / 255.F, Size(416, 416), Scalar(), true, false);
		net.setInput(inputBlob, "data");                   //set the network input
		Mat detectionMat = net.forward("detection_out");   //compute output
		float confidenceThreshold = 0.24; //by default
		float defalut_distance = 30.0;

		for (int i = 0; i < detectionMat.rows; i++) {
			const int probability_index = 5;
			const int probability_size = detectionMat.cols - probability_index;
			float* prob_array_ptr = &detectionMat.at<float>(i, probability_index);
			size_t objectClass = max_element(prob_array_ptr, prob_array_ptr + probability_size) - prob_array_ptr;


			//특정한 물체가 detection된 확률
			float confidence = detectionMat.at<float>(i, (int)objectClass + probability_index);
			//For drawing
			if (confidence > confidenceThreshold) {
				float x_center = detectionMat.at<float>(i, 0) * frame.cols;
				float y_center = detectionMat.at<float>(i, 1) * frame.rows;
				float width = detectionMat.at<float>(i, 2) * frame.cols;
				float height = detectionMat.at<float>(i, 3) * frame.rows;

				Point p1(cvRound(x_center - width / 2), cvRound(y_center - height / 2));
				Point p2(cvRound(x_center + width / 2), cvRound(y_center + height / 2));
				Rect object(p1, p2);
				Scalar object_roi_color(0, 255, 0);
				rectangle(frame, object, object_roi_color);

				String className = objectClass < classNamesVec.size() ? classNamesVec[objectClass] : format("unknown(%d)", objectClass);
				String label = format("%s: %.2f", className.c_str(), confidence);
				int baseLine = 0;
				Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
				rectangle(frame, Rect(p1, Size(labelSize.width, labelSize.height + baseLine)), object_roi_color, FILLED);
				putText(frame, label, p1 + Point(0, labelSize.height), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));

				//Point front_car(x_center, p2.y) ;

				Point my_car(frame.size().width / 2 - 60 ,  frame.size().height - 30);
				line(frame, my_car, my_car, Scalar(0, 0, 255), 2);

				vector <Scalar> front_object(3);
				front_object[0] = Scalar(0, 0, 255);
				front_object[1] = Scalar(0, 255, 255);
				front_object[2] = Scalar(0, 100, 200);

				if (p1.x <= my_car.x && my_car.x <= p2.x) {
					float distance = my_car.y - p2.y;
					if (distance < defalut_distance)
						rectangle(frame, object, front_object[0]);		//smaller
					else if(distance == defalut_distance)
						rectangle(frame, object, front_object[1]);		//same
					else
						rectangle(frame, object, front_object[2]);		//larger

				}

			}
		}

		imshow("YOLO: Detections", frame);

		if (waitKey(1) >= 0)
			break;
	}
	return 0;
}