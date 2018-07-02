#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace cv;

Mat rgb;
char window_name[20] = "Pixel value demo";

static void onMouse(int event, int i, int j, int f, void* param)
{
	Mat &rgb = *((Mat*)param);
	Vec3b pix = rgb.at<Vec3b>(j, i);
	int red = rgb.at<Vec3b>(j, i)[2];
	int green = rgb.at<Vec3b>(j, i)[1];
	int blue = rgb.at<Vec3b>(j, i)[0];
	cout << "X= " << i << " Y= " << j << " Red= " << red << " Green= " << green << " Blue= " << blue << endl;
}
int main()
{
	rgb = imread("lena.jpg");
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	imshow(window_name, rgb);
	setMouseCallback(window_name, onMouse, &rgb);
	waitKey(0);
	destroyAllWindows();
	return 0;
}