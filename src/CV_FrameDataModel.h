#pragma once

#include "IFrameDataModel.h"

#include <opencv2/core/mat.hpp>
using namespace cv;

/**
 * @brief The FrameDataModel class manages the acquired frames.
 * Even if it's not the job for a model to convert Mat frames to QPixmap, it also does it for convenience.
 */
class CV_FrameDataModel : public IFrameDataModel<Mat>
{
protected:
    unsigned char* getData(Mat& frame) override;
    uint getCols(Mat& frame) override;
    uint getRows(Mat& frame) override;

private:
    std::mutex mMutex;
    std::deque<Mat> mFramesBuffer;
    VideoConfig mSrcConfig;
};
