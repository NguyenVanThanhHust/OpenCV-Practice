#include "Header.h"

#define FOR(i,s,e) for(int i = s ; i < e ; i++)

static void onMouse2(int event, int i, int j, int f, void* param)
{
	Mat &init_lap = *((Mat*)param);
	Vec3b pix = init_lap.at<Vec3b>(j, i);
	int red = init_lap.at<Vec3b>(j, i)[2];
	int green = init_lap.at<Vec3b>(j, i)[1];
	int blue = init_lap.at<Vec3b>(j, i)[0];
	cout << "X2= " << i << " Y2= " << j << " Red2= " << red << " Green2= " << green << " Blue2= " << blue << endl;
}



Mat ConSobel(Mat img, int Sx[3][3], int Sy[3][3])
{
	Mat tem;
	img.convertTo(tem, CV_32F);

	// Sobel X and Sobel Y
	Mat res1 = Mat::zeros(img.size(), tem.type());
	Mat res2 = Mat::zeros(img.size(), tem.type());
	Mat res3 = Mat::zeros(img.size(), tem.type());

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
					
					res1.at<float>(i, j) += tem.at<float>(xn, yn)*Sx[a][b];
					res2.at<float>(i, j) += tem.at<float>(xn, yn)*Sy[a][b];
				}
			}
		}
	}


	// For each point in new image
	for (int i = 1; i < img.rows - 1; i++)
	{
		for (int j = 1; j < img.cols - 1; j++)
		{
			res3.at<float>(i, j) = sqrt(res1.at<float>(i, j)*res1.at<float>(i, j) + res2.at<float>(i, j)*res2.at<float>(i, j));
		}
	}

	res1.convertTo(res1, CV_8U);
	res2.convertTo(res2, CV_8U);
	res3.convertTo(res3, CV_8U);
	return res3;
}

Mat ConSobel2(Mat img, int Sx[3][3], int Sy[3][3])
{
	Mat tem;
	img.convertTo(tem, CV_32F);

	// Sobel X and Sobel Y
	Mat res1 = Mat::zeros(img.size(), tem.type());
	Mat res2 = Mat::zeros(img.size(), tem.type());
	Mat res3 = Mat::zeros(img.size(), tem.type());

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
					
					res1.at<float>(i, j) = res1.at<float>(i, j) + tem.at<float>(xn, yn)*Sx[a][b];
					res2.at<float>(i, j) = res2.at<float>(i, j) + tem.at<float>(xn, yn)*Sy[a][b];
				}
			}
		}
	}


	// For each point in new image
	for (int i = 1; i < img.rows - 1; i++)
	{
		for (int j = 1; j < img.cols - 1; j++)
		{
			res3.at<float>(i, j) = sqrt(res1.at<float>(i, j)*res1.at<float>(i, j) + res2.at<float>(i, j)*res2.at<float>(i, j));
		}
	}

	res1.convertTo(res1, CV_8U);
	res2.convertTo(res2, CV_8U);
	res3.convertTo(res3, CV_8U);
	return res3;
}

int main()
{
	// Input image 
	Mat gray_image = imread("patheon.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	// define Laplacian
	int L[3][3];
	{
		L[1][1] = -4;
		L[0][0] = L[0][2] = L[2][0] = L[2][2] = 0;
		L[0][1] = L[1][0] = L[1][2] = L[2][1] = 1;
	}

	// define Gx for Sobel X 
	int Sx[3][3];
	{
		Sx[0][0] = -3;
		Sx[0][1] = 0;
		Sx[0][2] = 3;
		Sx[1][0] = -10;
		Sx[1][1] = 0;
		Sx[1][2] = 10;
		Sx[2][0] = -3;
		Sx[2][1] = 0;
		Sx[2][2] = 3;
	}

	// define Sy for Sobel Y 
	int Sy[3][3];
	{
		Sy[0][0] = -3;
		Sy[0][1] = -10;
		Sy[0][2] = -3;
		Sy[2][0] = 3;
		Sy[2][1] = 10;
		Sy[2][2] = 3;
		Sy[1][0] = Sy[1][1] = Sy[1][2] = 0;
	}


	/*
	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	imshow(window_name, laplacian);
	setMouseCallback(window_name, onMouse, &laplacian);
	*/
	waitKey(0);
	return 0;
}