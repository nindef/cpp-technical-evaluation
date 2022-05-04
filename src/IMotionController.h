#pragma once

#include "IFrameDataModel.h"

#include <iostream>
#include <motion_detector/include/motion_detector.hpp>

template<typename FT, typename FW>
class IMotionController
{
public:
    /**
     * @brief setMotionDetector Sets the motion detector used to detect differences between cv::Mat frames.
     * @param motionDetector The motion detector shared pointer, as a cv::Mat frames motion detector.
     */
    void setMotionDetector(std::shared_ptr<MockMotionDetector<FT>> motionDetector)
    {
        mMotionDetector = motionDetector;
    }

    /**
     * @brief setDataModel Sets the acquired frames model.
     * @param model The acquired frames data model shared pointer
     */
    void setDataModel(std::shared_ptr<IFrameDataModel<FT>> model)
    {
        mDataModel = model;
    }

    /**
     * @brief setFrameWriter Sets the frame writer
     * @param frameWriter The frame writer shared pointer
     */
    void setFrameWriter(std::shared_ptr<FW> frameWriter)
    {
        mFrameWriter = frameWriter;
    }

    /**
     * @brief startMotionDetection Starts efectively the motion detection process.
     * @param [in] threadRunning This flag tells when this process has to stop
     * @param [in] secondsAfterMotionFinishes This value represents the seconds
     * @param [out] isRecording This value states the if the controller is writing video after motion has been detected
     */
    void startMotionDetection (bool* threadRunning, int* secondsAfterMotionFinishes, bool* isRecording)
    {
        assert(threadRunning != nullptr);
        assert(secondsAfterMotionFinishes != nullptr);
        assert(isRecording != nullptr);
        assert(mMotionDetector != nullptr);
        assert(mFrameWriter != nullptr);

        auto numFramesAfterMotionUndetected = *secondsAfterMotionFinishes * mFrameWriter->getFPS();
        while (*threadRunning)
        {
            FT first, second;
            if (mDataModel->getFirstTwoFrames(first, second))
            {
                const auto motionDetected = (mMotionDetector->detect(first, second) == Motion::MOTION);

                if (motionDetected)
                {
                    *isRecording = true;
                    numFramesAfterMotionUndetected = *secondsAfterMotionFinishes * mFrameWriter->getFPS();
                    mFrameWriter->writeFrame(std::move(second));
                }
                else if (numFramesAfterMotionUndetected > 0)
                {
                    *isRecording = true;

                    //write during N frames more
                    numFramesAfterMotionUndetected--;
                    mFrameWriter->writeFrame(std::move(second));
                }
                else
                {
                    *isRecording = false;
                }
                mDataModel->freeNFrames(1);
            }
        }
        mFrameWriter->closeFile ();
    }

protected:
    std::shared_ptr<IFrameDataModel<FT>> mDataModel;
    std::shared_ptr<FW> mFrameWriter;
    std::shared_ptr<MockMotionDetector<FT>> mMotionDetector;
    FT framePlayed;
};
