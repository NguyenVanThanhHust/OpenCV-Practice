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

int main()
{
    VideoCapture cap("./test_video.mp4");
    cv::HOGDescriptor hog;
    hog.load("./model/car_detection.yml");
    while(1){
        Mat frame;
        // Capture frame-by-frame
        cap >> frame;
    
        // If the frame is empty, break immediately
        if (frame.empty())
        break;

        std::vector<cv::Rect> detections;
        std::vector<double> foundWeights;
        hog.detectMultiScale(frame, detections, foundWeights);

        for (size_t i = 0; i < detections.size(); i++)
        {
            Point pt1 = detections.at(i).tl();
            Point pt2 = detections.at(i).br();
            cv::rectangle(frame, pt1, pt2, cv::Scalar(0, 255, 0));
        }
        
        // Display the resulting frame
        imshow( "Frame", frame );

        // Press  ESC on keyboard to exit
        char c=(char)waitKey(25);
        if(c==27)
        break;
        }
        
        // When everything done, release the video capture object
        cap.release();

        // Closes all the frames
        destroyAllWindows();
            
        return 0;
}