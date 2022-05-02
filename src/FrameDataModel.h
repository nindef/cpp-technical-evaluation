#pragma once

#include <deque>
#include <mutex>
#include <opencv2/core/mat.hpp>


using namespace cv;

class FrameDataModel
{
public:
    struct SourceVideoConfig{
        double fourcc = 0;
        double fps = 0;
        Size size = {0,0};

        SourceVideoConfig (){}
        SourceVideoConfig (double fourcc, double fps, Size size)
        {
            this->fourcc = fourcc;
            this->fps = fps;
            this->size = size;
        }
    };

    FrameDataModel ();
    ~FrameDataModel () = default;

    void setSrcVideoConfig (double fourcc, double fps, Size size);
    FrameDataModel::SourceVideoConfig& getSrcVideoConfig();

    void storeFrame(Mat frame);
    Mat getFrame (int frameIndex);

    bool getFirstTwoFrames (Mat& first, Mat& second);
    void freeNFrames (int numFramesToFree);

private:
    std::mutex mMutex;
    std::deque<Mat> mFramesBuffer;
    SourceVideoConfig mSrcConfig;
};
