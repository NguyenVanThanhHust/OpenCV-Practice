#include <iostream>
#include <string>
#include <time.h>
//include opencv core
#include "opencv2\core\core.hpp"
// #include "opencv2\contrib\contrib.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"

//file handling
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;

//Functions
int VideoDisplay();
Mat GrabCut(Mat image);

const unsigned int BORDER = 5;
const unsigned int BORDER2 = BORDER + BORDER;

int main()
{
	int value = VideoDisplay();
	system("pause");
	return 0;
}
Mat GrabCut(Mat image)
{
	clock_t tStart_all = clock();
	cv::Mat result; // segmentation result (4 possible values)
	cv::Mat bgModel, fgModel; // the models (internally used)
							  // downsample the image
	cv::Mat downsampled;
	cv::pyrDown(image, downsampled, cv::Size(image.cols / 2, image.rows / 2));
	cv::Rect rectangle(BORDER, BORDER, downsampled.cols - BORDER2, downsampled.rows - BORDER2);

	clock_t tStart = clock();
	// GrabCut segmentation
	cv::grabCut(downsampled,    // input image
		result,   // segmentation result
		rectangle,// rectangle containing foreground
		bgModel, fgModel, // models
		1,        // number of iterations
		cv::GC_INIT_WITH_RECT); // use rectangle
	printf("Time taken by GrabCut with downsampled image: %f s\n", (clock() - tStart) / (double)CLOCKS_PER_SEC);

	// Get the pixels marked as likely foreground
	cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
	// upsample the resulting mask
	cv::Mat resultUp;
	cv::pyrUp(result, resultUp, cv::Size(result.cols * 2, result.rows * 2));
	// Generate output image
	cv::Mat foreground(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));
	image.copyTo(foreground, resultUp); // bg pixels not copied
	return foreground;
}

int  VideoDisplay() {

	cout << "start recognizing..." << endl;
	//lbpcascades/lbpcascade_frontalface.xml
	string classifier = "C:/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml";

	CascadeClassifier face_cascade;
	string window = "Capture - face detection";

	if (!face_cascade.load(classifier)) {
		cout << " Error loading file" << endl;
		return -1;
	}
	VideoCapture cap(0);
	//VideoCapture cap("C:/Users/lsf-admin/Pictures/Camera Roll/video000.mp4");

	if (!cap.isOpened())
	{
		cout << "exit" << endl;
		return -1;
	}

	//double fps = cap.get(CV_CAP_PROP_FPS);
	//cout << " Frames per seconds " << fps << endl;
	namedWindow(window, 1);
	long count = 0;
	int fps = 0;
	//Start and end times
	time_t start, end;
	//Start the clock
	time(&start);
	int counter = 0;



	while (true)
	{
		vector<Rect> faces;
		Mat frame;
		Mat graySacleFrame;
		Mat original;

		cap >> frame;

		time(&end);
		++counter;
		double sec = difftime(end, start);
		fps = counter / sec;

		if (!frame.empty()) {

			//clone from original frame
			original = frame.clone();

			//convert image to gray scale and equalize
			cvtColor(original, graySacleFrame, CV_BGR2GRAY);
			//equalizeHist(graySacleFrame, graySacleFrame);

			//detect face in gray image
			face_cascade.detectMultiScale(graySacleFrame, faces, 1.1, 3, 0, cv::Size(90, 90));

			//number of faces detected
			//cout << faces.size() << " faces detected" << endl;
			std::string frameset = std::to_string(fps);
			std::string faceset = std::to_string(faces.size());

			int width = 0, height = 0;
			cv::Mat seg_grabcut;
			//region of interest

			for (int i = 0; i < faces.size(); i++)
			{
				//region of interest
				Rect face_i = faces[i];

				////crop the roi from grya image
				//Mat face = graySacleFrame(face_i);
				Mat crop = original(face_i);
				////resizing the cropped image to suit to database image sizes
				Mat face_resized;
				cv::resize(crop, face_resized, Size(512, 512), 1.0, 1.0, INTER_CUBIC);


				//drawing green rectagle in recognize face
				rectangle(original, face_i, CV_RGB(0, 255, 0), 1);

				if (!face_resized.empty())
				{
					seg_grabcut = GrabCut(face_resized);
					if (!seg_grabcut.empty())
					{
						imshow("segmented result", seg_grabcut);
					}

				}

			}


			putText(original, "Frames/Second: " + frameset, Point(30, 60), CV_FONT_HERSHEY_COMPLEX_SMALL, 1.0, CV_RGB(0, 255, 0), 1.0);
			//display to the winodw
			cv::imshow(window, original);

			//cout << "model infor " << model->getDouble("threshold") << endl;

		}

		if (waitKey(30) >= 0) break;
	}
}