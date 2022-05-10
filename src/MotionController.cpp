#include "MotionController.h"


void MotionController::setMotionDetector(std::shared_ptr<MockMotionDetector<std::shared_ptr<IFrameType> > > motionDetector)
{
    mMotionDetector = motionDetector;
}

void MotionController::setDataModel(std::shared_ptr<FrameDataModel> model)
{
    mDataModel = model;
}

void MotionController::setFrameWriter(std::shared_ptr<AFrameWriter> frameWriter)
{
    mFrameWriter = frameWriter;
}

void MotionController::startMotionDetection(bool *threadRunning, int *secondsAfterMotionFinishes, bool *isRecording)
{
    assert(threadRunning != nullptr);
    assert(secondsAfterMotionFinishes != nullptr);
    assert(isRecording != nullptr);
    assert(mMotionDetector != nullptr);
    assert(mFrameWriter != nullptr);

    auto numFramesAfterMotionUndetected = *secondsAfterMotionFinishes * mFrameWriter->getFPS();
    while (*threadRunning)
    {
        const auto framesPair = mDataModel->getFirstTwoFrames ();
        if (framesPair.first != nullptr && framesPair.second != nullptr)
        {
            const auto motionDetected = (mMotionDetector->detect(framesPair.first, framesPair.second) == Motion::MOTION);

            if (motionDetected)
            {
                *isRecording = true;
                numFramesAfterMotionUndetected = *secondsAfterMotionFinishes * mFrameWriter->getFPS();
                mFrameWriter->writeFrame(framesPair.second);
            }
            else if (numFramesAfterMotionUndetected > 0)
            {
                *isRecording = true;

                //write during N frames more
                numFramesAfterMotionUndetected--;
                mFrameWriter->writeFrame(framesPair.second);
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
