#include "FrameDataModel.h"


void FrameDataModel::setSrcVideoConfig(double fourcc, double fps, Size size)
{
    mSrcConfig = VideoConfig(fourcc, fps, size);
}

FrameDataModel::VideoConfig& FrameDataModel::getSrcVideoConfig()
{
    return mSrcConfig;
}

void FrameDataModel::storeFrame(Mat frame)
{
    mMutex.lock();
    mFramesBuffer.push_back(std::move(frame));
    mMutex.unlock();
}

bool FrameDataModel::getCurrentFramePixmap(QPixmap& pixmap, QSize dstSize)
{
    Mat frame;
    mMutex.lock();
    if (!mFramesBuffer.empty())
    {
        frame = mFramesBuffer[0];
        QImage image((unsigned char*) frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
        image = image.rgbSwapped ();
        image = image.scaled(dstSize, Qt::KeepAspectRatio);
        pixmap = QPixmap::fromImage(image);

        mMutex.unlock();
        return true;
    }

    mMutex.unlock();
    return false;
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

void FrameDataModel::freeAllFrames()
{
    mMutex.lock();
    mFramesBuffer.clear();
    mMutex.unlock();
}
