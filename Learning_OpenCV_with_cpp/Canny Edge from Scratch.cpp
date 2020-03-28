#include "Header.h"

Mat myCanny(Mat &img, int upperThres, int lowerThres, double size = 3)
{
	

	// Calculate the gradient magnitudes and direction
	Mat magX = Mat(img.size(), CV_32F);
	Mat magY = Mat(img.size(), CV_32F);
	Sobel(img, magX, CV_32F, 1, 0, size);
	Sobel(img, magY, CV_32F, 0, 1, size);

	
	// Calculate slope at each points = dividing the Y derivative by X
	Mat direction = Mat(img.size(), CV_32F);
	divide(magY, magX, direction);

	// Calculate the magnitude of gradient in each pixel
	Mat sum = Mat(img.rows, img.cols, CV_64F);
	Mat prodX = Mat(img.rows, img.cols, CV_64F);
	Mat prodY = Mat(img.rows, img.cols, CV_64F);
	multiply(magX, magX, prodX);
	multiply(magY, magY, prodY);
	sum = prodX + prodY;
	sqrt(sum, sum);

	// Nonmaximum suppression
	// This step is to figure which point lie on edges
	// This kind of point have gradient magnitude greater than upper threshold
	Mat returnImg = Mat(img.size(), CV_32F);
	// Initialize iterators
	MatIterator_<float>itMag = sum.begin<float>();
	MatIterator_<float>itDirection = direction.begin<float>();
	MatIterator_<float>itRet = returnImg.begin<float>();
	MatIterator_<float>itend = sum.end<float>();

	for (; itMag != itend; ++itDirection, ++itRet, ++itMag)
	{
		const cv::Point pos = itRet.pos();

		float currentDirection = atan(*itDirection) * 180 / 3.142;

		while (currentDirection<0) currentDirection += 180;

		*itDirection = currentDirection;

		if (*itMag<upperThres) continue;
		bool flag = true;

		if (currentDirection>112.5 && currentDirection <= 157.5)
		{
			if (pos.y>0 && pos.x<img.cols - 1 && *itMag <= sum.at<float>(pos.y - 1, pos.x + 1)) flag = false;
			if (pos.y<img.rows - 1 && pos.x>0 && *itMag <= sum.at<float>(pos.y + 1, pos.x - 1)) flag = false;
		}
		else if (currentDirection>67.5 && currentDirection <= 112.5)
		{
			if (pos.y>0 && *itMag <= sum.at<float>(pos.y - 1, pos.x)) flag = false;
			if (pos.y<img.rows - 1 && *itMag <= sum.at<float>(pos.y + 1, pos.x)) flag = false;
		}
		else if (currentDirection > 22.5 && currentDirection <= 67.5)
		{
			if (pos.y>0 && pos.x>0 && *itMag <= sum.at<float>(pos.y - 1, pos.x - 1)) flag = false;
			if (pos.y<img.rows - 1 && pos.x<img.cols - 1 && *itMag <= sum.at<float>(pos.y + 1, pos.x + 1)) flag = false;
		}
		else
		{
			if (pos.x>0 && *itMag <= sum.at<float>(pos.y, pos.x - 1)) flag = false;
			if (pos.x<img.cols - 1 && *itMag <= sum.at<float>(pos.y, pos.x + 1)) flag = false;
		}
		if (flag)
		{
			*itRet = 255;
		}
	}
	// Step 4: Hysterysis threshold
	bool imageChanged = true;
	int i = 0;
	while (imageChanged)
	{
		imageChanged = false;
		//cout << "Iteration " << i;
		i++;
		itMag = sum.begin<float>();
		itDirection = direction.begin<float>();
		itRet = returnImg.begin<float>();
		itend = sum.end<float>();
		for (; itMag != itend; ++itMag, ++itDirection, ++itRet)
		{
			Point pos = itRet.pos();
			if (pos.x<2 || pos.x>img.cols - 2 || pos.y<2 || pos.y>img.rows - 2)
				continue;
			float currentDirection = *itDirection;
			// Do we have a pixel we already know as an edge?
			if (*itRet == 255)
			{
				*itRet = (float)64;
				if (currentDirection>112.5 && currentDirection <= 157.5)
				{
					if (pos.y>0 && pos.x>0)
					{
						if (lowerThres <= sum.at<float>(pos.y - 1, pos.x - 1) &&
							returnImg.at<float>(pos.y - 1, pos.x - 1) != 64 &&
							direction.at<float>(pos.y - 1, pos.x - 1) > 112.5 &&
							direction.at<float>(pos.y - 1, pos.x - 1) <= 157.5 &&
							sum.at<float>(pos.y - 1, pos.x - 1) > sum.at<float>(pos.y - 2, pos.x) &&
							sum.at<float>(pos.y - 1, pos.x - 1) > sum.at<float>(pos.y, pos.x - 2))
						{
							returnImg.ptr<float>(pos.y - 1, pos.x - 1)[0] = 255;
							imageChanged = true;
						}
					}
					if (pos.y<img.rows - 1 && pos.x<img.cols - 1)
					{
						if (lowerThres <= sum.at<float>(cv::Point(pos.x + 1, pos.y + 1)) &&
							returnImg.at<float>(pos.y + 1, pos.x + 1) != 64 &&
							direction.at<float>(pos.y + 1, pos.x + 1) > 112.5 &&
							direction.at<float>(pos.y + 1, pos.x + 1) <= 157.5 &&
							sum.at<float>(pos.y - 1, pos.x - 1) > sum.at<float>(pos.y + 2, pos.x) &&
							sum.at<float>(pos.y - 1, pos.x - 1) > sum.at<float>(pos.y, pos.x + 2))
						{
							returnImg.ptr<float>(pos.y + 1, pos.x + 1)[0] = 255;
							imageChanged = true;
						}
					}
				}
				else if (currentDirection>67.5 && currentDirection <= 112.5)
				{
					if (pos.x>0)
					{
						if (lowerThres <= sum.at<float>(cv::Point(pos.x - 1, pos.y)) &&
							returnImg.at<float>(pos.y, pos.x - 1) != 64 &&
							direction.at<float>(pos.y, pos.x - 1) > 67.5 &&
							direction.at<float>(pos.y, pos.x - 1) <= 112.5 &&
							sum.at<float>(pos.y, pos.x - 1) > sum.at<float>(pos.y - 1, pos.x - 1) &&
							sum.at<float>(pos.y, pos.x - 1) > sum.at<float>(pos.y + 1, pos.x - 1))
						{
							returnImg.ptr<float>(pos.y, pos.x - 1)[0] = 255;
							imageChanged = true;
						}
					}
					if (pos.x<img.cols - 1)
					{
						if (lowerThres <= sum.at<float>(cv::Point(pos.x + 1, pos.y)) &&
							returnImg.at<float>(pos.y, pos.x + 1) != 64 &&
							direction.at<float>(pos.y, pos.x + 1) > 67.5 &&
							direction.at<float>(pos.y, pos.x + 1) <= 112.5 &&
							sum.at<float>(pos.y, pos.x + 1) > sum.at<float>(pos.y - 1, pos.x + 1) &&
							sum.at<float>(pos.y, pos.x + 1) > sum.at<float>(pos.y + 1, pos.x + 1))
						{
							returnImg.ptr<float>(pos.y, pos.x + 1)[0] = 255;
							imageChanged = true;
						}
					}
				}
				else if (currentDirection > 22.5 && currentDirection <= 67.5)
				{
					if (pos.y>0 && pos.x<img.cols - 1)
					{
						if (lowerThres <= sum.at<float>(cv::Point(pos.x + 1, pos.y - 1)) &&
							returnImg.at<float>(pos.y - 1, pos.x + 1) != 64 &&
							direction.at<float>(pos.y - 1, pos.x + 1) > 22.5 &&
							direction.at<float>(pos.y - 1, pos.x + 1) <= 67.5 &&
							sum.at<float>(pos.y - 1, pos.x + 1) > sum.at<float>(pos.y - 2, pos.x) &&
							sum.at<float>(pos.y - 1, pos.x + 1) > sum.at<float>(pos.y, pos.x + 2))
						{
							returnImg.ptr<float>(pos.y - 1, pos.x + 1)[0] = 255;
							imageChanged = true;
						}
					}
					if (pos.y<img.rows - 1 && pos.x>0)
					{
						if (lowerThres <= sum.at<float>(cv::Point(pos.x - 1, pos.y + 1)) &&
							returnImg.at<float>(pos.y + 1, pos.x - 1) != 64 &&
							direction.at<float>(pos.y + 1, pos.x - 1) > 22.5 &&
							direction.at<float>(pos.y + 1, pos.x - 1) <= 67.5 &&
							sum.at<float>(pos.y + 1, pos.x - 1) > sum.at<float>(pos.y, pos.x - 2) &&
							sum.at<float>(pos.y + 1, pos.x - 1) > sum.at<float>(pos.y + 2, pos.x))
						{
							returnImg.ptr<float>(pos.y + 1, pos.x - 1)[0] = 255;
							imageChanged = true;
						}
					}
				}
				else
				{
					if (pos.y>0)
					{
						if (lowerThres <= sum.at<float>(cv::Point(pos.x, pos.y - 1)) &&
							returnImg.at<float>(pos.y - 1, pos.x) != 64 &&
							(direction.at<float>(pos.y - 1, pos.x) < 22.5 ||
								direction.at<float>(pos.y - 1, pos.x) >= 157.5) &&
							sum.at<float>(pos.y - 1, pos.x) > sum.at<float>(pos.y - 1, pos.x - 1) &&
							sum.at<float>(pos.y - 1, pos.x) > sum.at<float>(pos.y - 1, pos.x + 2))
						{
							returnImg.ptr<float>(pos.y - 1, pos.x)[0] = 255;
							imageChanged = true;
						}
					}
					if (pos.y<img.rows - 1)
					{
						if (lowerThres <= sum.at<float>(cv::Point(pos.x, pos.y + 1)) &&
							returnImg.at<float>(pos.y + 1, pos.x) != 64 &&
							(direction.at<float>(pos.y + 1, pos.x) < 22.5 ||
								direction.at<float>(pos.y + 1, pos.x) >= 157.5) &&
							sum.at<float>(pos.y + 1, pos.x) > sum.at<float>(pos.y + 1, pos.x - 1) &&
							sum.at<float>(pos.y + 1, pos.x) > sum.at<float>(pos.y + 1, pos.x + 1))
						{
							returnImg.ptr<float>(pos.y + 1, pos.x)[0] = 255;
							imageChanged = true;
						}
					}
				}
			}
		}
	}
	cv::MatIterator_<float>current = returnImg.begin<float>();    cv::MatIterator_<float>final = returnImg.end<float>();
	for (; current != final; ++current)
	{
		if (*current == 64)
			*current = 255;
	}
	return returnImg;
}

int main()
{
	Mat img = imread("patheon.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	/*
	Mat img2;
	double upt = threshold(img, img2, 0, 255, THRESH_OTSU);
	int dd = (int)upt;
	Mat result = myCanny(img, dd, dd/2);
	imshow("result", result);
	*/

	Mat result2 = myCanny(img, 80, 25);
	//Mat result3 = myCanny(img, 120, 40);
	imshow("result2 ", result2);
	//imshow("sss", result3);

	waitKey(0);
	return 0;
}