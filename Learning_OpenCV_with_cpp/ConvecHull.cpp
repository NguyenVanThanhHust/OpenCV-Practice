#include <opencv2\opencv.hpp>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

// Create a function to draw approximation set of contour
vector<Point> contoursConvexHull(vector<vector<Point>> contours)
{
	vector<Point> result;
	vector<Point> pts;

	for (size_t i = 0; i < contours.size(); i++)
	{
		for (size_t j = 0; j < contours.size(); i++)
		{
			pts.push_back(contours[i][j]);
		}
	}
	convexHull(pts, result);
	return result;
}


int main()
{
	Mat src, srcGray, srcBlur, srcCanny;
	src = imread("C://Users//Thanh BK//Pictures//Screenshots//pantheon.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cvtColor(src, srcGray, CV_BGR2GRAY);
	Size ksize = Size(3, 3);
	blur(srcGray, srcCanny, ksize);

	Canny(srcBlur, srcCanny, 0, 100, 3, true);

	vector<vector<Point>> contours;
	findContours(srcCanny, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	Mat drawing = Mat::zeros(srcCanny.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(255, 255, 255);
		drawContours(drawing, contours, i, color, 2);
	}

	vector<Point> ConvexHullPoints = contoursConvexHull(contours);

	polylines(drawing, ConvexHullPoints, true, Scalar(0, 0, 255), 2);
	imshow("Contours", drawing);

	polylines(src, ConvexHullPoints, true, Scalar(0, 0, 255), 2);
	imshow("contoursConvexHull", src);
	waitKey();
	return 0;




}