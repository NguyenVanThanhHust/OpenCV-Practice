#include <opencv2\opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

int main()
{
	// Read input image and convert to float
	Mat src = imread("C://Users//Thanh BK//Pictures//Screenshots//eye.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dst;
	distanceTransform(src, dst, DIST_L2, 3);
	imshow("Orginal", src);
	imshow("result", dst);
	waitKey(0);
	return 0;
}