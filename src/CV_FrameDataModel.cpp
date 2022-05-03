#include "CV_FrameDataModel.h"

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
