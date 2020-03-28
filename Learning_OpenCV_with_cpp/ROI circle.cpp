#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

string nameIMG = "anh xuoc/WIN_20180915_125416";
string duoi = ".jpg";

Mat circleROI(Mat &img, Point center, int radius)
{
	Mat plotImage = img;

	// (badly handpicked coords):
	//Point cen(img.cols/2, img.rows/2);
	//int radius = 200;
	Point cen = center;
	//get the Rect containing the circle:
	Rect r(cen.x - radius, cen.y - radius, radius * 2, radius * 2);

	// obtain the image ROI:
	Mat roi(plotImage, r);

	// make a black mask, same size:
	Mat mask(roi.size(), roi.type(), Scalar::all(0));
	// with a white, filled circle in it:
	circle(mask, Point(radius, radius), radius, Scalar::all(255), -1);

	// combine roi & mask:
	Mat eye_cropped = roi & mask;
	return eye_cropped;
}

int main()
{
	return 0;
}
	
	