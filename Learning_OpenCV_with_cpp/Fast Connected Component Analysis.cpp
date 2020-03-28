// Example 14-3. Drawing labeled connected components

#include <algorithm>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main() {

	cv::Mat img, img_edge, labels, centroids, img_color, stats;

	// load image or show help if no image was provided
	img = imread("C://Users//Thanh BK//Pictures//Screenshots//square.png", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Original", img);
	cv::threshold(img, img_edge, 128, 255, cv::THRESH_BINARY);
	cv::imshow("Image after threshold", img_edge);

	int i, nccomps = cv::connectedComponentsWithStats(
		img_edge,
		labels,
		stats,
		centroids
	);
	cout << "Total Connected Components Detected: " << nccomps << endl;

	vector<cv::Vec3b> colors(nccomps + 1);
	colors[0] = cv::Vec3b(0, 0, 0); // background pixels remain black.
	for (i = 1; i <= nccomps; i++) {
		colors[i] = cv::Vec3b(rand() % 256, rand() % 256, rand() % 256);
		if (stats.at<int>(i - 1, cv::CC_STAT_AREA) < 100)
			colors[i] = cv::Vec3b(0, 0, 0); // small regions are painted with black too.
	}
	img_color = cv::Mat::zeros(img.size(), CV_8UC3);
	for (int y = 0; y < img_color.rows; y++)
		for (int x = 0; x < img_color.cols; x++)
		{
			int label = labels.at<int>(y, x);
			CV_Assert(0 <= label && label <= nccomps);
			img_color.at<cv::Vec3b>(y, x) = colors[label];
		}

	cv::imshow("Labeled map", img_color);
	cv::waitKey();
	return 0;
}

