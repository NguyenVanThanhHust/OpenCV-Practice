#include <opencv2\opencv.hpp>
#include <opencv2\video.hpp>
#include <opencv2\highgui.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

int main()
{
	// Create a VideoCapture object and use camera to capture the video
	VideoCapture cap(0);

	// Check if camera opened successfully
	if (!cap.isOpened())
	{
		cout << "Error opening video stream" << endl;
		return -1;
	}

	// Initialize background substractor
	Ptr<BackgroundSubtractorMOG2> bg = createBackgroundSubtractorMOG2();

	// Create empty input img, fg and background image and fg mask
	Mat img, fgMask, bgImage, fgImg;

	for(;;)
	{
		

		// Capture frame-by-img 
		cap >> img;

		// If the img is empty, break immediately
		if (img.empty())
			break;

		// Display the resulting img    
		imshow("img", img);
		
		cap.retrieve(img, CV_CAP_OPENNI_BGR_IMAGE);
		// Create fg mask of propersize
		if (fgMask.empty())
		{
			fgMask.create(img.size(), img.type());
		}

		// Compute fg mask 8 bit image
		// -1 is parameter that choose automatically your learning rate
		bg->apply(img, fgMask, -1);

		// Smoothe the mask to reduce noise in image
		GaussianBlur(fgMask, fgMask, Size(5, 5), 3.5, 3.5);

		//threshold mak to sature at black and while values
		threshold(fgMask, fgMask, 10, 255, THRESH_BINARY);

		// Create black foreground image
		fgImg = Scalar::all(0);

		// Copy source image to foreground image only in area with white maks
		img.copyTo(fgImg, fgMask);

		// Get the background image
		bg->getBackgroundImage(bgImage);

		// Show the result
		//imshow("Foreground Mask", fgMask);
		imshow("Foreground Image", fgImg);

		// Press  ESC on keyboard to  exit
		char c = (char)waitKey(1);
		if (c == 27)
			break;
	}
	
	return 0;
}
