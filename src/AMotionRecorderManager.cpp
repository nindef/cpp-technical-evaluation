#include "AMotionRecorderManager.h"

#include "AFrameAcquisitor.h"
#include "FrameDataModel.h"
#include "MotionController.h"


AMotionRecorderManager::AMotionRecorderManager(std::string sourceStreamPath, std::string cameraName) :
    mSourceStreamPath (sourceStreamPath), mOutputVideoBaseName("rec_"+cameraName), mCameraName(cameraName)
{
    mCallbackFunction = [=](bool){};
}

void AMotionRecorderManager::startMotionDetection()
{
    if (!mMembersBuilt)
        build();

    mThreadRunning = true;
    mIsRecording = false;
    mThreadFrameAcquisition = new std::thread (&AFrameAcquisitor::runAcquisition, mFrameAcquisitor, &mThreadRunning);
    mThreadMotionController = new std::thread (&MotionController::startMotionDetection, mMotionController, &mThreadRunning,
                                               &mSecondsAfterMotionFinishes, &mIsRecording);

    mCallbackFunction(mThreadRunning);
}

void AMotionRecorderManager::stopMotionDetection ()
{
    if (mThreadRunning && mMembersBuilt)
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

void AMotionRecorderManager::setCallbackFunctionOnStatusChange(std::function<void (bool)> callbackFunction)
{
    mCallbackFunction = callbackFunction;
}

void AMotionRecorderManager::unsetCallbackFunctionOnStatusChange()
{
    mCallbackFunction = [=](bool){};
}

void AMotionRecorderManager::setSecondsAfterMotionFinishes(int seconds)
{
    mSecondsAfterMotionFinishes = seconds;
}

const std::string &AMotionRecorderManager::getSourceStreamPath() const
{
    return mSourceStreamPath;
}

bool AMotionRecorderManager::isRecordingVideo() const
{
    return mIsRecording;
}

bool AMotionRecorderManager::isMotionDetectionRunning() const
{
    return mThreadRunning;
}

bool AMotionRecorderManager::getCurrentFrame(QPixmap &pixmap, QSize size)
{
    if (mThreadRunning && mMembersBuilt)
    {
        const auto frame = mDataModel->getFirstFrame ();
        if (frame != nullptr)
        {
            pixmap = frame->getCurrentFramePixmap(size);
            return true;
        }
    }

    return false;
}

void AMotionRecorderManager::build()
{
    buildMembers ();
    mMembersBuilt = true;
}

