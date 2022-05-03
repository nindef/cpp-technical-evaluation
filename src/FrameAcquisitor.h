#pragma once

#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

class FrameDataModel;

class FrameAcquisitor
{
public:
    FrameAcquisitor ();
    ~FrameAcquisitor();

    void setDataModel(std::shared_ptr<FrameDataModel> model);
    void configure (std::string sourceStreamPath, VideoCaptureAPIs videoCaptureAPI);
    void runAcquisition (bool* frameControlRunning);

private:
    bool *mThreadRunning = nullptr;
    std::shared_ptr<VideoCapture> mVideoCapture;
    std::shared_ptr<FrameDataModel> mDataModel;
};
