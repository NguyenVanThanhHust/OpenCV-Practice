#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

string nameIMG = "anh nut//1";
string duoi = ".jpg";

Mat img;
int threshval = 72;// 77;//42

static void on_trackbar(int, void*)
{
	Mat bw = threshval < 128 ? (img < threshval) : (img > threshval);
	Mat labelImage(img.size(), CV_32S);
	int nLabels = connectedComponents(bw, labelImage, 8);
	std::vector<Vec3b> colors(nLabels);
	colors[0] = Vec3b(0, 0, 0);//background
	for (int label = 1; label < nLabels; ++label) {
		colors[label] = Vec3b((rand() & 255), (rand() & 255), (rand() & 255));
	}
	Mat dst(img.size(), CV_8UC3);
	for (int r = 0; r < dst.rows; ++r) {
		for (int c = 0; c < dst.cols; ++c) {
			int label = labelImage.at<int>(r, c);
			Vec3b &pixel = dst.at<Vec3b>(r, c);
			pixel = colors[label];
		}
	}
	imshow("Connected Components", dst);
}

Mat connectedMat(Mat img, int threshold)
{
	Mat bw = threshold < 128 ? (img < threshold) : (img > threshold);
	Mat labelImage(img.size(), CV_32S);
	int nLabels = connectedComponents(bw, labelImage, 8);
	std::vector<Vec3b> colors(nLabels);
	colors[0] = Vec3b(0, 0, 0);//background
	for (int label = 1; label < nLabels; ++label) {
		colors[label] = Vec3b((rand() & 255), (rand() & 255), (rand() & 255));
	}
	Mat dst(img.size(), CV_8UC3);
	for (int r = 0; r < dst.rows; ++r) {
		for (int c = 0; c < dst.cols; ++c) {
			int label = labelImage.at<int>(r, c);
			Vec3b &pixel = dst.at<Vec3b>(r, c);
			pixel = colors[label];
		}
	}
	return dst;
}

int main()
{

	img = imread(nameIMG + duoi, IMREAD_GRAYSCALE);

	// Remember, to resize is just to visuallize
	// when we use connected component on real image 
	// we don't need to resize
	Mat imageMat;
	Mat imageMat2 = img;
	resize(imageMat2, imageMat, Size(imageMat2.cols*0.25, imageMat2.rows*0.25), 0, 0, CV_INTER_CUBIC);
	img = imageMat;
	namedWindow("Connected Components", WINDOW_AUTOSIZE);
	createTrackbar("Threshold", "Connected Components", &threshval, 255, on_trackbar);
	on_trackbar(threshval, 0);
	
	waitKey(0);
	return EXIT_SUCCESS;
}
