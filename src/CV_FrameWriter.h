#pragma once

#include "AFrameWriter.h"

#include <iostream>

#include "opencv2/videoio.hpp"
using namespace cv;

/**
 * @brief The CV_FrameWriter class manages the process to write frames into a video file using OpenCV
 */
class CV_FrameWriter : public AFrameWriter
{
public:
    CV_FrameWriter ();

    /**
     * @brief writeFrame Writes the frame into the video by using the VideoWriter.
     * If the VideoWriter hasn't opened yet the video file, this method opens it
     * @param frame The frame to write into the video.
     */
    void writeFrame(std::shared_ptr<IFrameType> frame) override;

    /**
     * @brief closeFile Closes this file (so, releases the VideoWriter)
     */
    void closeFile () override;

protected:
    std::shared_ptr<VideoWriter> mVideoWriter;
};
