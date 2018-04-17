// Contain good Features to Track and cornerSubPix

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat src = imread("E://Visual Studio Project//OpenCV Project/index.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	src.convertTo(src, CV_8UC1);
	// define some parameters of function 
	vector<Point2f> corners;
	int maxCorners = 50;
	double qualityLevel = 0.1;
	double minDistance = 0;
	int blockSize = 3;

	goodFeaturesToTrack(src, corners, maxCorners, qualityLevel, minDistance, noArray(), blockSize, false);

	// draw the tracked corner
	int numCor = corners.size();
	Mat src2 = imread("E://Visual Studio Project//OpenCV Project/index.jpg", CV_LOAD_IMAGE_COLOR);
	for (int i = 0; i < numCor; i++)
	{
		circle(src2, corners[i], 5, Scalar(0, 255, 0), 2, 8);
	}

	
	imshow("image", src2);
	waitKey(0);
	return 0;
}