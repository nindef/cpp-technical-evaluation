#include "FrameDataModel.h"


FrameDataModel::FrameDataModel()
{

}

void FrameDataModel::setSrcVideoConfig(double fourcc, double fps, Size size)
{
    mSrcConfig = SourceVideoConfig(fourcc, fps, size);
}

FrameDataModel::SourceVideoConfig& FrameDataModel::getSrcVideoConfig()
{
    return mSrcConfig;
}

void FrameDataModel::storeFrame(Mat frame)
{
    mMutex.lock();
    mFramesBuffer.push_back(std::move(frame));
    mMutex.unlock();
}

Mat FrameDataModel::getFrame(int frameIndex)
{
    Mat frame;
    mMutex.lock();
    if (!mFramesBuffer.empty() && frameIndex < mFramesBuffer.size())
    {
        frame = mFramesBuffer[frameIndex];
    }
    mMutex.unlock();
    return frame;
}

bool FrameDataModel::getFirstTwoFrames(Mat &first, Mat &second)
{
    bool areFramesSet = false;
    mMutex.lock();
    if (mFramesBuffer.size() >= 2)
    {
        first = mFramesBuffer[0];
        second = mFramesBuffer[1];
        areFramesSet = true;
    }
    mMutex.unlock();
    return areFramesSet;
}

void FrameDataModel::freeNFrames (int numFramesToFree)
{
    mMutex.lock();
    for (auto index = 0; index < numFramesToFree; index++)
    {
        if (!mFramesBuffer.empty())
            mFramesBuffer.pop_front();
    }
    mMutex.unlock();
}
