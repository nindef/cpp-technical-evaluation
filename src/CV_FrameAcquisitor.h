#pragma once

#include <iostream>
#include "FrameDataModel.h"
#include "AFrameAcquisitor.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

/**
 * @brief The FrameAcquisitor class holds a VideoCapture object to read frames
 * from an stream and store them into the data model.
 */
class CV_FrameAcquisitor : public AFrameAcquisitor
{
public:
    /**
     * @brief CV_FrameAcquisitor Default constructor. Initializes the shared pointer to the VideoCapture
     */
    CV_FrameAcquisitor ();

    /**
     * @brief ~CV_FrameAcquisitor Default destructor. Releases the VideoCapture if exists
     */
    ~CV_FrameAcquisitor ();

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

private:
    std::shared_ptr<VideoCapture> mVideoCapture;
};
