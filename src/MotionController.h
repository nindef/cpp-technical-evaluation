#pragma once

#include "AFrameWriter.h"
#include "FrameDataModel.h"

#include <iostream>
#include <motion_detector/include/motion_detector.hpp>

class MotionController
{
public:
    /**
     * @brief setMotionDetector Sets the motion detector used to detect differences between cv::Mat frames.
     * @param motionDetector The motion detector shared pointer, as a cv::Mat frames motion detector.
     */
    void setMotionDetector(std::shared_ptr<MockMotionDetector<std::shared_ptr<IFrameType>>> motionDetector);

    /**
     * @brief setDataModel Sets the acquired frames model.
     * @param model The acquired frames data model shared pointer
     */
    void setDataModel(std::shared_ptr<FrameDataModel> model);

    /**
     * @brief setFrameWriter Sets the frame writer
     * @param frameWriter The frame writer shared pointer
     */
    void setFrameWriter(std::shared_ptr<AFrameWriter> frameWriter);

    void setCameraName (std::string cameraName);

    /**
     * @brief startMotionDetection Starts efectively the motion detection process.
     * @param [in] threadRunning This flag tells when this process has to stop
     * @param [in] secondsAfterMotionFinishes This value represents the seconds
     * @param [out] isRecording This value states the if the controller is writing video after motion has been detected
     */
    void startMotionDetection (bool* threadRunning, int* secondsAfterMotionFinishes, bool* isRecording);

protected:
    std::shared_ptr<FrameDataModel> mDataModel;
    std::shared_ptr<AFrameWriter> mFrameWriter;
    std::shared_ptr<MockMotionDetector<std::shared_ptr<IFrameType>>> mMotionDetector;
    std::string mCameraName;
};
