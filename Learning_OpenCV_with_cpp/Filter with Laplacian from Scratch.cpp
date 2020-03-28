#include "Header.h"

// Define a pixel
typedef  Point_<float> Pixel;


Mat ConLap(Mat img, int kerLap[3][3])
{
	Mat tem;
	img.convertTo(tem, CV_32F);

	Mat res1 = Mat::zeros(img.size(), tem.type());
	

	// For each point in new image
	for (int i = 1; i < img.rows - 1; i++)
	{
		for (int j = 1; j < img.cols - 1; j++)
		{
			// For each point in Kernel
			for (int a = 0; a < 3; a++)
			{
				for (int b = 0; b < 3; b++)
				{
					int xn = i + a - 1;
					int yn = j + b - 1;
					res1.at<float>(i, j) = res1.at<float>(i, j) + tem.at<float>(xn, yn)*kerLap[a][b];
				}
			}
		}
	}



	res1.convertTo(res1, CV_8U);
	
	return res1;
}


int main()
{
	// Input image 
	Mat gray_image = imread("patheon.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	waitKey(0);
	return 0;
}