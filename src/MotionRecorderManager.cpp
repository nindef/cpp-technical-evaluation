#include "MotionRecorderManager.h"

#include "CV_FrameAcquisitor.h"
#include "CV_MotionController.h"
#include "CV_FrameWriter.h"
#include "CV_FrameDataModel.h"


MotionRecorderManager::MotionRecorderManager(std::string sourceStreamPath, std::string outputVideoBaseName) : mSourceStreamPath (sourceStreamPath)
{
    mDataModel = std::make_shared<CV_FrameDataModel>();
    const auto frameWriter = std::make_shared<CV_FrameWriter>();
    frameWriter->setDataModel(mDataModel);
    frameWriter->setOutputVideoBaseName(outputVideoBaseName);
    const auto motionDetector = std::make_shared<MockMotionDetector<Mat>>();

    mFrameAcquisitor = std::make_shared<CV_FrameAcquisitor>();
    mFrameAcquisitor->setDataModel(mDataModel);
    mFrameAcquisitor->configure(sourceStreamPath);

    mMotionController = std::make_shared<CV_MotionController>();
    mMotionController->setMotionDetector(motionDetector);
    mMotionController->setDataModel(mDataModel);
    mMotionController->setFrameWriter(frameWriter);
}

void MotionRecorderManager::startMotionDetection()
{
    mThreadRunning = true;
    mMotionDetected = false;
    mThreadFrameAcquisition = new std::thread (&CV_FrameAcquisitor::runAcquisition, mFrameAcquisitor, &mThreadRunning);
    mThreadMotionController = new std::thread (&CV_MotionController::startMotionDetection, mMotionController, &mThreadRunning,
                                               &mSecondsAfterMotionFinishes, &mMotionDetected);

    mCallbackFunction(mThreadRunning);
}

void MotionRecorderManager::stopMotionDetection ()
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

void MotionRecorderManager::setCallbackFunctionOnStatusChange(std::function<void (bool)> callbackFunction)
{
    mCallbackFunction = callbackFunction;
}

void MotionRecorderManager::setSecondsAfterMotionFinishes(int seconds)
{
    mSecondsAfterMotionFinishes = seconds;
}

const std::string &MotionRecorderManager::getSourceStreamPath() const
{
    return mSourceStreamPath;
}

bool MotionRecorderManager::isMotionDetected() const
{
    return mMotionDetected;
}

bool MotionRecorderManager::isMotionDetectionRunning() const
{
    return mThreadRunning;
}

bool MotionRecorderManager::getCurrentFrame(QPixmap &pixmap, QSize size)
{
    if (mThreadRunning)
        return mDataModel->getCurrentFramePixmap(pixmap, size);

    return false;
}

