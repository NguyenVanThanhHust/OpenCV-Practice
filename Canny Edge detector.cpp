#include <opencv2\opencv.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

int main()
{
	// Read input image and convert to float
	Mat src = imread("C://Users//Thanh BK//Pictures//Screenshots//eye.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	GaussianBlur(src, src, Size(5, 5), 0, 0);
	vector<Vec3f> circles;
	
	HoughCircles(src, circles, HOUGH_GRADIENT, 2, src.cols / 2);
	size_t s = 0;
	for (size_t i = 0; i < circles.size(); ++i)
	{
		cout << circles[i][0] << " " << circles[i][1] << " " << circles[i][2] << " " << endl;
		circle(src, Point(cvRound(circles[i][0]), cvRound(circles[i][1])),
			cvRound(circles[i][2]),
			Scalar(0, 0, 255),
			2,
			CV_AA
		);
		s = s + i;
	}
	cout << s;
	Mat dst;
	//Canny(src, dst, 20, 150, 3, false);
	imshow("Hough Circles", src);
	
	waitKey(0);
	return 0;
}