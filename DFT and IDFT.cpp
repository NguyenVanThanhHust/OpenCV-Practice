// How to use opencv to acclerate computation of convolutions

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	// Read input image and convert to float
	Mat src = imread("C://Users//Thanh BK//Pictures//Screenshots//eye.jpg", CV_LOAD_IMAGE_COLOR);
	src.convertTo(src, CV_32FC3, 1 / 255.0);

	// Patch size
	Size patchSize(100, 100);
	Point topleft(src.cols / 2, src.rows / 2);
	Rect roi(topleft.x, topleft.y, patchSize.width, patchSize.height);
	Mat src2 = src(roi);

	int dft_M = getOptimalDFTSize(src.rows + src2.rows - 1);
	int dft_N = getOptimalDFTSize(src.cols + src2.cols - 1);

	Mat dft_A = Mat::zeros(dft_M, dft_N, CV_32F);
	Mat dft_B = Mat::zeros(dft_M, dft_N, CV_32F);

	Mat dft_A_part = dft_A(Rect(0, 0, src.cols, src.rows));
	Mat dft_B_part = dft_B(Rect(0, 0, src2.cols, src2.rows));

	src.convertTo(dft_A_part, dft_A_part.type(), 1, -mean(src)[0]);
	src2.convertTo(dft_B_part, dft_B_part.type(), 1, -mean(src2)[0]);
	
	/*
	// For Discrete fourier transform 
	dft(dft_A, dft_A, 0, src.rows);
	dft(dft_B, dft_B, 0, src2.rows);
	*/

	// For Discrete cosine transform
	dct(dft_A, dft_A, src.rows);
	dct(dft_B, dft_B, src2.rows);

	mulSpectrums(dft_A, dft_B, dft_A, 0, true);
	idft(dft_A, dft_A, DFT_SCALE, src.rows + src2.rows - 1);

	Mat corr = dft_A(Rect(0, 0, src.cols + src2.cols - 1, src.rows + src2.rows - 1));
	normalize(corr, corr, 0, 1, NORM_MINMAX, corr.type());
	pow(corr, 3., corr);

	src2 ^= Scalar::all(255);
	imshow("Image", src);
	imshow("Correlation", corr);
	waitKey(0);
	return 0;

}