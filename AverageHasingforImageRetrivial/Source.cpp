#include <opencv2\opencv_modules.hpp>
#include <opencv2\opencv.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;
using namespace cv;

static string bin_to_hex(vector<int> v);

int main()
{
	std::vector<cv::String> vFile;
	cv::glob("Flower", vFile);

	vector<float> value;
	for (int i = 0; i < (int)vFile.size(); i++)
	{
		//Mat src = imread("rose_test.jpg");
		std::string filePath = vFile.at(i);

		Mat src = imread(filePath);

		Mat resizedMat;
		resize(src, resizedMat, Size(32, 32), 0, 0);

		Mat grayMat;
		cvtColor(resizedMat, grayMat, CV_RGB2GRAY);

		Mat grayMatFloat;
		grayMat.convertTo(grayMatFloat, CV_32F);
		
		Mat dctMat;
		dct(grayMatFloat, dctMat );

		//in case inverse
		//dct(dctMat, grayMatFloat, DCT_INVERSE );
		vector<int> bit_value;
		Scalar mean_value = mean(dctMat, Mat());

		for (size_t i = 0; i < dctMat.cols; i++)
		{
			for (size_t j = 0; j < dctMat.rows; j++)
			{
				if (dctMat.at<float>(j, i) >= mean_value[0])

				{
					bit_value.push_back(1);
				}
				else
				{
					bit_value.push_back(0);
				}
			}
		}
		vector<string> hexa_value;

		for (int i = 0; i < 64; i++)
		{
			vector<int> tem_v;
			for (int j = 0; j < 4; j++)
			{
				int k = bit_value.at(i * 4 + j);
				tem_v.push_back(k);

			}
			string tem_str = bin_to_hex(tem_v);
			hexa_value.push_back(tem_str);
		}
		std::ofstream myfile;
		myfile.open("example_dct.txt", ios::out | ios::app | ios::binary);
		myfile << filePath << "   \n";
		myfile << "\n";
		std::ostream_iterator<std::string> output_iterator(myfile, "");
		myfile << "\n";
		std::copy(hexa_value.begin(), hexa_value.end(), output_iterator);
	}
	
	
	return 0;
}

string bin_to_hex(vector<int> v)
{
	int result = v[0] * 8 + v[1] * 4 + v[2] * 2 + v[3] * 1;
	if (result <= 9)
	{
		string s = to_string(result);
		return s;
	}
	else
	{
		vector<string> tem_str = { "a","b","c","d","e","f" };
		int k = result - 10;
		string res = tem_str[k];
		return res;
	}
	

}