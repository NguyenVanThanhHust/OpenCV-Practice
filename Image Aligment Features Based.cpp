/* 
Main ideas : 
Detect the features of image by ORB : Oriented FAST and Rotated BRIEF
Find the matching features in two images
Calculate Homographu
Warping image
*/
#include <opencv2\opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2\features2d.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

const int MAX_FEATURES = 500;
const float GOOD_MATCH_PERCENT = 0.15f;

void alignImages(Mat &img1, Mat &img2, Mat &imReg, Mat &EsHo)
{
	// Convert image to gray scale
	Mat imgGray1, imgGray2;
	cvtColor(img1, imgGray1, CV_BGR2GRAY);
	cvtColor(img2, imgGray2, CV_BGR2GRAY);

	// Variables to store keypoints and descriptors
	std::vector<KeyPoint> keypoint1, keypoint2;
	Mat descriptor1, descriptor2;

	// Detect ORB featues and compute descriptors
	Ptr<Feature2D> orb = ORB::create(MAX_FEATURES);
	orb->detectAndCompute(imgGray1, Mat(), keypoint1, descriptor1);
	orb->detectAndCompute(imgGray2, Mat(), keypoint2, descriptor2);

	// Match features
	std::vector<DMatch> matches;
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
	matcher->match(descriptor1, descriptor2, matches, Mat());

	// Sort matches by score
	std::sort(matches.begin(), matches.end());

	// Remove not so good matches
	const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
	matches.erase(matches.begin() + numGoodMatches, matches.end());

	// Draw top mathces
	Mat imMatches;
	drawMatches(img1, keypoint1, img2, keypoint2, matches, imMatches);
	imwrite("matches.jpg", imMatches);

	// Extract location of good matches
	vector<Point2f> points1, points2;
	for (size_t i = 0; i < matches.size(); i++)
	{
		points1.push_back(keypoint1[matches[i].queryIdx].pt);
		points2.push_back(keypoint2[matches[i].trainIdx].pt);
	}

	// Find the homography
	EsHo = findHomography(points1, points2, RANSAC);

	// Use the homography to warp image
	warpPerspective(img1, imReg, EsHo, img2.size());
}

int main()
{
	// The refernce image and image to be aligned 
	Mat src = imread("C://Users//Thanh BK//Pictures//Screenshots//form.jpg", CV_LOAD_IMAGE_ANYCOLOR);
	Mat algImg= imread("C://Users//Thanh BK//Pictures//Screenshots//scanned-form.jpg", CV_LOAD_IMAGE_ANYCOLOR);

	// Registered image will be restored in imReg
	// Estimated homography will be restored in EsHo
	Mat imReg, EsHo;

	alignImages(src, algImg, imReg, EsHo);
	waitKey(0);
	return 0;
}
