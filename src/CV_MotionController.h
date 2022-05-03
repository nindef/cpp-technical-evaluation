#pragma once

#include "CV_FrameWriter.h"
#include "IMotionController.h"

#include <iostream>

/**
 * @brief The MotionController class requests if there is motion detected between two OpenCV frames.
 * Depending on that, writes the frame into the FrameWriter.
 */
class CV_MotionController : public IMotionController<Mat, CV_FrameWriter>
{};
