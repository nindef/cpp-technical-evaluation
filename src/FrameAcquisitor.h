#pragma once

#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;

class FrameDataModel;

class FrameAcquisitor
{
public:
    FrameAcquisitor (std::shared_ptr<FrameDataModel> model);
    ~FrameAcquisitor();

    void runAcquisition (bool* frameControlRunning);

private:
    bool *mThreadRunning = nullptr;
    std::shared_ptr<VideoCapture> mVideoCapture;
    std::shared_ptr<FrameDataModel> mDataModel;
};
