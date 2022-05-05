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
public:

    /**
     * @brief getCurrentFramePixmap Gets the current frame (frame in first place in deque) as a QPixmap
     * @param pixmap The destiny QPixmap
     * @param dstSize The desired size for the resulting pixmap.
     * The image will keep its aspect ratio so this is actually the maximum size for the pixmap, in the best case.
     * @return True if the pixmap has been successfully create, false otherwise.
     */
    bool getCurrentFramePixmap(QPixmap& pixmap, QSize dstSize) override;
protected:
    unsigned char* getData(Mat& frame) override;
    uint getCols(Mat& frame) override;
    uint getRows(Mat& frame) override;
};
