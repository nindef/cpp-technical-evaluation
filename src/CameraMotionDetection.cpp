#include "CameraMotionDetection.h"
#include "FrameAcquisitor.h"
#include "FrameDataModel.h"
#include "FrameWriter.h"
#include "MotionController.h"


CameraMotionDetection::CameraMotionDetection(std::string sourceStreamPath, std::string outputVideoBaseName) : mSourceStreamPath (sourceStreamPath)
{
    mDataModel = std::make_shared<FrameDataModel>();
    const auto frameWriter = std::make_shared<FrameWriter>();
    frameWriter->setDataModel(mDataModel);
    frameWriter->setOutputVideoBaseName(outputVideoBaseName);
    const auto motionDetector = std::make_shared<MockMotionDetector<Mat>>();
    motionDetector->initializeMotionVector();

    mFrameAcquisitor = std::make_shared<FrameAcquisitor>();
    mFrameAcquisitor->setDataModel(mDataModel);
    mFrameAcquisitor->configure(sourceStreamPath, cv::CAP_FFMPEG);

    mMotionController = std::make_shared<MotionController>();
    mMotionController->setMotionDetector(motionDetector);
    mMotionController->setDataModel(mDataModel);
    mMotionController->setFrameWriter(frameWriter);
}

void CameraMotionDetection::startMotionDetection()
{
    mThreadRunning = true;
    mMotionDetected = false;
    mThreadFrameAcquisition = new std::thread (&FrameAcquisitor::runAcquisition, mFrameAcquisitor, &mThreadRunning);
    mThreadMotionController = new std::thread (&MotionController::startMotionDetection, mMotionController, &mThreadRunning,
                                               &mSecondsAfterMotionFinishes, &mMotionDetected);

    mCallbackFunction(mThreadRunning);
}

void CameraMotionDetection::stopMotionDetection ()
{
    if (mThreadRunning)
    {
        mThreadRunning = false;
        if (mThreadFrameAcquisition && mThreadMotionController)
        {
            mThreadMotionController->join();
            mThreadFrameAcquisition->join();

            mDataModel->freeAllFrames();

            delete mThreadMotionController;
            delete mThreadFrameAcquisition;
        }
    }
    mCallbackFunction(mThreadRunning);
}

void CameraMotionDetection::setCallbackFunctionOnStatusChange(std::function<void (bool)> callbackFunction)
{
    mCallbackFunction = callbackFunction;
}

void CameraMotionDetection::setSecondsAfterMotionFinishes(int seconds)
{
    mSecondsAfterMotionFinishes = seconds;
}

const std::string &CameraMotionDetection::getSourceStreamPath() const
{
    return mSourceStreamPath;
}

bool CameraMotionDetection::isMotionDetected() const
{
    return mMotionDetected;
}

bool CameraMotionDetection::isMotionDetectionRunning() const
{
    return mThreadRunning;
}

bool CameraMotionDetection::getCurrentFrame(QPixmap &pixmap, QSize size)
{
    if (mThreadRunning)
        return mDataModel->getCurrentFramePixmap(pixmap, size);

    return false;
}

