#pragma once


#include <opencv2/core/mat.hpp>
#include <thread>

class FrameAcquisitor;
class FrameDataModel;

using namespace cv;

class FrameController
{
public:
    FrameController (std::shared_ptr<FrameDataModel> model);

    void runFrameControl (bool* frameControlRunning);

//    void stopFrameControl();

private:
    bool *mThreadRunning = nullptr;
    std::shared_ptr<FrameDataModel> mDataModel;
    std::shared_ptr<FrameAcquisitor> mFrameAcquisitor;
    std::thread threadFrameControl;

    void getAndStoreFrame ();
};
