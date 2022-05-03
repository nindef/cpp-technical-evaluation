#pragma once

#include "opencv2/videoio.hpp"
#include <iostream>

using namespace cv;

class FrameDataModel;

class FrameWriter
{
public:
    FrameWriter ();
    ~FrameWriter () {};

    void setOutputVideoBaseName (const std::string& outputVideoBaseName);
    void setDataModel(std::shared_ptr<FrameDataModel> model);
    void writeFrame(Mat frame);

    void closeFile ();

private:
    bool *mRunning = nullptr;
    std::string mOutputVideoBaseName;
    std::shared_ptr<FrameDataModel> mDataModel;
    std::shared_ptr<VideoWriter> mVideoWriter;
};
