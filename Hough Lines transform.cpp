/*
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main()
{
Mat src, gray;
src = imread("C://Users//Thanh BK//Pictures//Screenshots//eye.jpg", 1);
cvtColor(src, gray, CV_BGR2GRAY);

// Reduce the noise so we avoid false circle detection
GaussianBlur(gray, gray, Size(9, 9), 2, 2);

vector<Vec3f> circles;

// Apply the Hough Transform to find the circles
HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1, 30, 200, 50, 0, 0);

// Draw the circles detected
for (size_t i = 0; i < circles.size(); i++)
{
Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
int radius = cvRound(circles[i][2]);
circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);// circle center
circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);// circle outline
cout << "center : " << center << "\nradius : " << radius << endl;
}

// Show your results
namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
imshow("Hough Circle Transform Demo", src);

waitKey(0);
return 0;
}
*/



#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat src = imread("C://Users//Thanh BK//Pictures//Screenshots//patheon.jpg", 0);

	Mat dst, cdst;
	Canny(src, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

	vector<Vec2f> lines;
	// detect lines
	HoughLines(dst, lines, 1, CV_PI / 180, 150, 0, 0);

	// draw lines
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}

	imshow("source", src);
	imshow("detected lines", cdst);

	waitKey();
	return 0;
}