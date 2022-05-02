#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <motion_detector/include/motion_detector.hpp>

class FrameWriter;
class FrameDataModel;

using namespace cv;

class MotionController
{
public:
    MotionController ();

    void setMotionDetector(std::shared_ptr<MockMotionDetector<Mat> > motionDetector);

    void setDataModel(std::shared_ptr<FrameDataModel> model);

    void setFrameWriter(std::shared_ptr<FrameWriter> frameWriter);

    void startMotionDetection (bool* frameControlRunning);

private:
    bool *mRunning = nullptr;
    std::shared_ptr<FrameDataModel> mDataModel;
    std::shared_ptr<FrameWriter> mFrameWriter;
    std::shared_ptr<MockMotionDetector<Mat>> mMotionDetector;
    Mat framePlayed;
};
