#include "MotionController.h"

#include "FrameDataModel.h"
#include "FrameAcquisitor.h"
#include "FrameWriter.h"

MotionController::MotionController()
{
}

void MotionController::setDataModel (std::shared_ptr<FrameDataModel> model)
{
    mDataModel = model;
}

void MotionController::setMotionDetector(std::shared_ptr<MockMotionDetector<Mat>> motionDetector)
{
    mMotionDetector = motionDetector;
}

void MotionController::setFrameWriter(std::shared_ptr<FrameWriter> frameWriter)
{
    mFrameWriter = frameWriter;
}

void MotionController::startMotionDetection (bool* frameControlRunning)
{
    assert(frameControlRunning != nullptr);
    assert(mMotionDetector != nullptr);
    assert(mFrameWriter != nullptr);

    mRunning = frameControlRunning;

    auto numFramesAfterMotionUndetected = 30;
    while (*mRunning)
    {
        Mat first, second;
        if (mDataModel->getFirstTwoFrames(first, second))
        {
            const auto motionDetected = (mMotionDetector->detect(first, second) == Motion::MOTION);

            if (motionDetected)
            {
                numFramesAfterMotionUndetected = 30;
                std::cout << "motion detected" << std::endl;
                mFrameWriter->writeFrame(std::move(second));
                mDataModel->freeNFrames(1);
            }
            else if (numFramesAfterMotionUndetected > 0)
            {
                //write during N frames more
                numFramesAfterMotionUndetected--;
                mFrameWriter->writeFrame(std::move(second));
                mDataModel->freeNFrames(1);
            }
            else
            {
                std::cout << "motion not detected" << std::endl;
                mDataModel->freeNFrames(1);
            }
        }
    }
}
