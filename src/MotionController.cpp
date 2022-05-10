#include "MotionController.h"

#include <QDateTime>
#include <QDebug>
#include <QElapsedTimer>

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

void MotionController::setCameraName(string cameraName)
{
    mCameraName = cameraName;
}

void MotionController::startMotionDetection(bool *threadRunning, int *secondsAfterMotionFinishes, bool *isRecording)
{
    assert(threadRunning != nullptr);
    assert(secondsAfterMotionFinishes != nullptr);
    assert(isRecording != nullptr);
    assert(mMotionDetector != nullptr);
    assert(mFrameWriter != nullptr);

    mFrameWriter->configure(mDataModel->getSrcVideoConfig());

    //mock setup
    mMotionDetector->setFps(mFrameWriter->getFPS());
    mMotionDetector->initializeMotionVector();

    auto numFramesAfterMotionUndetected = *secondsAfterMotionFinishes * mFrameWriter->getFPS();
    auto isMotionDetected = false;

    auto numFramesRecorded = 0.;

    while (*threadRunning)
    {
        const auto framesPair = mDataModel->getFirstTwoFrames ();
        if (framesPair.first != nullptr && framesPair.second != nullptr)
        {
            const auto motionDetected = (mMotionDetector->detect(framesPair.first, framesPair.second) == Motion::MOTION);

            if (isMotionDetected != motionDetected)
            {
                const auto currentTS = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz");
                if ((isMotionDetected = motionDetected))
                {
                    qDebug ().nospace() << currentTS << " Motion detected";
                    numFramesRecorded = 0;
                }
                else
                {
                    const auto seconds = numFramesRecorded / mFrameWriter->getFPS();
                    qDebug ().nospace() << currentTS << " Motion not detected on camera " << QString::fromStdString(mCameraName) << ". Recorded "<< seconds << " seconds.";
                }
            }

            if (motionDetected)
            {
                numFramesRecorded++;
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
    qDebug () << "Closing file";
    mFrameWriter->closeFile ();
}
