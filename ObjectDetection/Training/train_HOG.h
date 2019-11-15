#pragma once
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include <opencv2/objdetect.hpp>

#include <iostream>
#include <time.h>
#include <chrono>
#include <thread>

using namespace cv;
using namespace cv::ml;
using namespace std;

class CTrainedHog
{
public:
    CTrainedHog();
    ~CTrainedHog();

private:
    vector< float > get_svm_detector(const Ptr< SVM >& svm);
    void convert_to_ml(const std::vector< Mat > & train_samples, Mat& trainData);
    void load_images(const String & dirname, vector< Mat > & img_lst, bool showImages);
    void sample_neg(const vector< Mat > & full_neg_lst, vector< Mat > & neg_lst, const Size & size);
    void computeHOGs(const Size wsize, const vector< Mat > & img_lst, vector< Mat > & gradient_lst, bool use_flip);
    void test_trained_detector(String obj_det_filename, String test_dir, String videofilename);

public:
    void run_trained_hog();
};
