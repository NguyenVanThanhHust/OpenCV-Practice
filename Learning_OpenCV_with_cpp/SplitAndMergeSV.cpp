vector<Mat> SplitAndMergeSV(Mat src)
{
	vector<Mat> result;
	if (!src.empty())
	{
		Mat HSV;
		cvtColor(src, HSV, COLOR_RGB2HSV);
		vector<Mat> hsv;
		split(HSV, hsv);
		Mat result_hue = hsv[0];
		Mat result_sature = hsv[1];
		Mat result_value = hsv[2];

		vector<Mat> hue;
		hue.push_back(result_hue);
		hue.push_back(result_hue);
		hue.push_back(result_hue);

		vector<Mat> sature;
		sature.push_back(result_sature);
		sature.push_back(result_sature);
		sature.push_back(result_sature);

		vector<Mat> value;
		value.push_back(result_value);
		value.push_back(result_value);
		value.push_back(result_value);

		//merge(hue, result_hue);
		merge(sature, result_sature);
		merge(value, result_value);


		//result.push_back(result_blue);
		result.push_back(result_sature);
		result.push_back(result_value);
		return result;
	}
	else
	{
		result.push_back(src);
		result.push_back(src);
	}

}
Mat SVVMat(Mat src)
{
	vector<Mat> result;
	Mat SVVresult;
	if (!src.empty())
	{
		Mat HSV;
		cvtColor(src, HSV, COLOR_RGB2HSV);
		vector<Mat> hsv;
		split(HSV, hsv);
		Mat result_hue = hsv[0];
		Mat result_sature = hsv[1];
		Mat result_value = hsv[2];

		vector<Mat> sature;
		sature.push_back(result_sature);
		sature.push_back(result_sature);
		sature.push_back(result_sature);

		vector<Mat> value;
		value.push_back(result_value);
		value.push_back(result_value);
		value.push_back(result_value);

		//merge(hue, result_hue);
		merge(sature, result_sature);
		merge(value, result_value);


		//result.push_back(result_blue);
		result.push_back(result_sature);
		result.push_back(result_value);
		result.push_back(result_value);
		merge(result,SVVresult );
		return SVVresult;
	}
	else
	{
		return src;
	}
}