#include "CV_FrameDataModel.h"


bool CV_FrameDataModel::getCurrentFramePixmap(QPixmap &pixmap, QSize dstSize)
{
    Mat frame;
    mMutex.lock();
    if (!mFramesBuffer.empty())
    {
        frame = mFramesBuffer[0];
        QImage image(getData(frame), getCols(frame), getRows(frame), QImage::Format_RGB888);
        image = image.rgbSwapped ();
        image = image.scaled(dstSize, Qt::KeepAspectRatio);
        pixmap = QPixmap::fromImage(image);

        mMutex.unlock();
        return true;
    }

    mMutex.unlock();
    return false;
}

unsigned char *CV_FrameDataModel::getData(Mat &frame)
{
    return static_cast<unsigned char*>(frame.data);
}

uint CV_FrameDataModel::getCols(Mat &frame)
{
    return frame.cols;
}

uint CV_FrameDataModel::getRows(Mat &frame)
{
    return frame.rows;
}
