#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <motion_detector/include/motion_detector.hpp>

class FrameWriter;
class FrameDataModel;

using namespace cv;

/**
 * @brief The MotionController class requests if there is motion detected between two frames.
 * Depending on that, writes the frame into the FrameWriter.
 */
class MotionController
{
public:
    /**
     * @brief setMotionDetector Sets the motion detector used to detect differences between cv::Mat frames.
     * @param motionDetector The motion detector shared pointer, as a cv::Mat frames motion detector.
     */
    void setMotionDetector(std::shared_ptr<MockMotionDetector<Mat> > motionDetector);

    /**
     * @brief setDataModel Sets the acquired frames model.
     * @param model The acquired frames data model shared pointer
     */
    void setDataModel(std::shared_ptr<FrameDataModel> model);

    /**
     * @brief setFrameWriter Sets the frame writer
     * @param frameWriter The frame writer shared pointer
     */
    void setFrameWriter(std::shared_ptr<FrameWriter> frameWriter);

    /**
     * @brief startMotionDetection Starts efectively the motion detection process.
     * @param [in] threadRunning This flag tells when this process has to stop
     * @param [in] secondsAfterMotionFinishes This value represents the seconds
     * @param [out] outMotionDetected This value states the if motion is being detected by the MockMotionDetector
     */
    void startMotionDetection (bool* threadRunning, int* secondsAfterMotionFinishes, bool* outMotionDetected);

private:
    std::shared_ptr<FrameDataModel> mDataModel;
    std::shared_ptr<FrameWriter> mFrameWriter;
    std::shared_ptr<MockMotionDetector<Mat>> mMotionDetector;
    Mat framePlayed;
};
