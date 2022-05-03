#pragma once

#include <iostream>
#include "IFrameDataModel.h"
#include "IFrameAcquisitor.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

/**
 * @brief The FrameAcquisitor class holds a VideoCapture object to read frames
 * from an stream and store them into the data model.
 */
class CV_FrameAcquisitor : public IFrameAcquisitor<Mat,VideoCapture>
{
public:

    /**
     * @brief configure This method is used to configure the VideoCapture object.
     * @param sourceStreamPath The source stream path.
     * @param videoCaptureAPI The video capture api used.
     */
    void configure (std::string sourceStreamPath) override;

    /**
     * @brief runAcquisition Starts the frame acquisition process.
     * @param [in] threadRunning This flag tells when this process has to stop
     */
    void runAcquisition (bool* threadRunning) override;
};
