#include <iostream> 
#include "opencv2\opencv.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\xfeatures2d.hpp"
#include "opencv2\stitching.hpp"
using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

// bo sung 
// 1 hien thi keypoint va matches  :da lam
// 2 blend 2 tam anh : chua lam
// 3 xoa duong vien : chua lam
Mat calculate_h_matrix(Mat image1, Mat image2, string key1, string key2, string match12)//, float &a, float &b)
{
	//-- Step 1: Detect the keypoints using SIFT Detector
	Mat gray_image1, gray_image2;
	cvtColor(image1, gray_image1, CV_BGR2GRAY);
	cvtColor(image2, gray_image2, CV_BGR2GRAY);

	int NumFea = 200;

	Ptr<Feature2D> detector = SIFT::create(NumFea);
	vector< KeyPoint > keypoints_object, keypoints_scene;

	//-- Step 2: Calculate descriptors (feature vectors)
	Mat descriptors_object, descriptors_scene;
	detector->detectAndCompute(gray_image1, Mat(), keypoints_object, descriptors_object);
	detector->detectAndCompute(gray_image2, Mat(), keypoints_scene, descriptors_scene);

	// Draw the keypoint
	drawKeypoints(image1, keypoints_object, image1, Scalar(0, 0, 255));
	drawKeypoints(image2, keypoints_scene, image2, Scalar(0, 0, 255));
	imshow(key1, image1);
	imshow(key2, image2);
	
	
	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	vector< DMatch > matches;
	matcher.match(descriptors_object, descriptors_scene, matches);

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints 
	for (int i = 0; i < descriptors_object.rows; i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}

	//-- Use only "good" matches (i.e. whose distance is less than 3*min_dist )
	vector< DMatch > good_matches;
	Mat result;

	for (int i = 0; i < descriptors_object.rows; i++)
	{
		if (matches[i].distance < 3 * min_dist)
		{
			good_matches.push_back(matches[i]);
		}
	}
	vector< Point2f > obj;
	vector< Point2f > scene;
	int sumb = 0;
	int sumc = 0;
	for (int i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt); 
		Point p1 = keypoints_object[good_matches[i].queryIdx].pt;
		char x = gray_image1.at<uchar>(p1);
		int b = (int)x;
		sumb = sumb + b;
			
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
		Point p2 = keypoints_scene[good_matches[i].queryIdx].pt;
		char y = gray_image2.at<uchar>(p2);
		int c = (int)y;
		sumc = sumc + c;
	}

	float sumbf = sumb;
	sumbf = sumbf / good_matches.size();
	float sumcf = sumc;
	sumcf = sumcf / good_matches.size();
	float alpha = sumbf / (sumbf + sumcf);
	float beta = 1 - alpha;
	a = alpha;
	b = beta;

	// Draw the matches
	Mat imMatches;
	drawMatches(image1, keypoints_object, image2, keypoints_scene, matches, imMatches);
	imshow(match12, imMatches);
		
	// Find the Homography Matrix for img 1 and img2
	Mat H = findHomography(scene, obj, RANSAC);
	return H;
}


Mat stitch_image(Mat image1, Mat image2, Mat H)
{
	Mat result;
	warpPerspective(image2, result, H, Size(image1.cols + image2.cols, image1.rows));
	Mat half(result, Rect(0, 0, image1.cols, image1.rows));
	return result;
}
void Draw_result(Mat image)
{
	Mat ig;
	cvtColor(image, ig, CV_BGR2GRAY);

	threshold(ig, ig, 25, 255, THRESH_BINARY);
	vector<vector<Point> > contours; // Vector for storing contour
	vector<Vec4i> hierarchy;
	findContours(ig, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); // Find the contours in the image
	int largest_area = 0;
	int largest_contour_index = 0;
	Rect bounding_rect;

	for (int i = 0; i< contours.size(); i++) // iterate through each contour.
	{
		double a = contourArea(contours[i], false);  //  Find the area of contour
		if (a>largest_area) {
			largest_area = a;
			largest_contour_index = i;                //Store the index of largest contour
			bounding_rect = boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
		}
	}

	image = image(Rect(bounding_rect.x, bounding_rect.y, bounding_rect.width*0.9, bounding_rect.height));
	
	Mat fin_res = image;
	imshow("fin_res", fin_res);

}

int main()
{
	Mat img1 = imread("S1.jpg", CV_LOAD_IMAGE_ANYCOLOR);
	Mat img2 = imread("S2.jpg", CV_LOAD_IMAGE_ANYCOLOR);
	string key1, key2, match12;
	key1 = "keypoint in image1";
	key2 = "keypoint in image2";
	match12 = "match in image1 and image2";
	Mat H12 = calculate_h_matrix(img1, img2, key1, key2, match12);
	Mat res12;
	res12 = stitch_image(img1, img2, H12);
	Draw_result(res12);
	
	
	
	Mat img3 = imread("S3.jpg", CV_LOAD_IMAGE_ANYCOLOR);
	string key22, key3, match23;
	key22 = "keypoint in image 2";
	key3 = "keypoint in image 3";
	match23 = "match in image 3 and image 3";
	Mat H23 = calculate_h_matrix(img2, img3,key22, key3,match23);
	Mat res23 = stitch_image(img3, img2, H23);
	Draw_result(res23);
	
	string key_f, key_s, match_fin;
	key_f = "keypoint in image 12";
	key_s = "keypoint in image 23";
	match_fin = "final result";
	Mat H123 = calculate_h_matrix(res12, res23, key_f, key_s, match_fin);
	Mat res123 = stitch_image(res23, res12, H123);
	Draw_result(res123);
	

	waitKey(0);

	return 0;
}
