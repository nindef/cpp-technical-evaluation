#pragma once

#include "IFrameWriter.h"
#include "IFrameDataModel.h"

#include <iostream>

#include "opencv2/videoio.hpp"
using namespace cv;

/**
 * @brief The CV_FrameWriter class manages the process to write frames into a video file using OpenCV
 */
class CV_FrameWriter : public IFrameWriter<Mat, VideoWriter>
{
public:
    /**
     * @brief writeFrame Writes the frame into the video by using the VideoWriter.
     * If the VideoWriter hasn't opened yet the video file, this method opens it
     * @param frame The frame to write into the video.
     */
    void writeFrame(Mat frame) override;

    /**
     * @brief closeFile Closes this file (so, releases the VideoWriter)
     */
    void closeFile () override;
};
