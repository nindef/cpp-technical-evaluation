#pragma once

#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;

class FrameDataModel;

class FrameWriter
{
public:
    FrameWriter ();
    ~FrameWriter ();

    void setDataModel(std::shared_ptr<FrameDataModel> model);
    void writeFrame(Mat frame);

private:
    bool *mRunning = nullptr;
    std::shared_ptr<FrameDataModel> mDataModel;
    std::shared_ptr<VideoWriter> mVideoWriter;
};
