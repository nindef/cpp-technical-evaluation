#pragma once

#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

class FrameDataModel;

/**
 * @brief The FrameAcquisitor class holds a VideoCapture object to read frames
 * from an stream and store them into the data model.
 */
class FrameAcquisitor
{
public:
    /**
     * @brief ~FrameAcquisitor Default destructor. Releases the VideoCapture object.
     */
    ~FrameAcquisitor();

    /**
     * @brief setDataModel Sets the acquired frames model.
     * @param model The acquired frames data model shared pointer
     */
    void setDataModel(std::shared_ptr<FrameDataModel> model);

    /**
     * @brief configure This method is used to configure the VideoCapture object.
     * @param sourceStreamPath The source stream path.
     * @param videoCaptureAPI The video capture api used.
     */
    void configure (std::string sourceStreamPath, VideoCaptureAPIs videoCaptureAPI);

    /**
     * @brief runAcquisition Starts the frame acquisition process.
     * @param [in] threadRunning This flag tells when this process has to stop
     */
    void runAcquisition (bool* threadRunning);

private:
    bool *mThreadRunning = nullptr;
    std::shared_ptr<VideoCapture> mVideoCapture;
    std::shared_ptr<FrameDataModel> mDataModel;
};
