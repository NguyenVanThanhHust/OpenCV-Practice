

#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace cv;

struct AreaCmp
{
	AreaCmp(const vector<float>& _areas) : areas(&_areas) {}
	bool operator()(int a, int b) const { return (*areas)[a] > (*areas)[b]; }
	const vector<float>* areas;

};

int main()
{
	// load image 
	Mat img, img_edge, img_color, img_2;
	
	img = imread("C://Users//Thanh BK//Pictures//Screenshots//pantheon.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	img_2.size() = img.size();
	threshold(img, img_edge, 120, 255, THRESH_BINARY);
	imshow("Original", img);
	imshow("After threshold", img_edge);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(img_edge, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);
	
	cout << "\n Hit any key to draw next contour\n";
	cout << "Total Contours detected " << contours.size() << endl;

	vector<int> sortIdx(contours.size());
	vector<float> areas(contours.size());

	for (int n = 0; n < (int)contours.size(); n++)
	{
		sortIdx[n] = n;
		areas[n] = contourArea(contours[n], false);
	}

	// sort the contours so that the largest contours go first
	sort(sortIdx.begin(), sortIdx.end(), AreaCmp(areas));
	for (int n = 0; n < sortIdx.size(); n++)
	{
		int idx = sortIdx[n];
		cvtColor(img, img_color, CV_GRAY2BGR);
		drawContours(img_color, contours, idx, Scalar(0, 0, 255), 2, 8, hierarchy, 0);// change this value to see what happen

		cout << "Contours #" << idx << ": areas=" << areas[idx] << " ,nvertices = " << contours[idx].size() << endl;

		imshow("result", img_color);

	}
	double length = arcLength(contours, true);
	cout << length << endl;



	waitKey(0);
	return 0;


}