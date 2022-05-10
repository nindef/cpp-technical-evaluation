#include "CV_MotionRecorderManager.h"

#include "CV_FrameAcquisitor.h"
#include "MotionController.h"
#include "CV_FrameWriter.h"
#include "FrameDataModel.h"


CV_MotionRecorderManager::CV_MotionRecorderManager(std::string sourceStreamPath, std::string cameraName) :
    AMotionRecorderManager (sourceStreamPath, cameraName)
{
}

void CV_MotionRecorderManager::buildMembers()
{
    mDataModel = std::make_shared<FrameDataModel>();
    const auto frameWriter = std::make_shared<CV_FrameWriter>();
    frameWriter->setDataModel(mDataModel);
    frameWriter->setOutputVideoBaseName(mOutputVideoBaseName);
    const auto motionDetector = std::make_shared<MockMotionDetector<std::shared_ptr<IFrameType>>>();

    mFrameAcquisitor = std::make_shared<CV_FrameAcquisitor>();
    mFrameAcquisitor->setDataModel(mDataModel);
    mFrameAcquisitor->configure(mSourceStreamPath);

    mMotionController = std::make_shared<MotionController>();
    mMotionController->setMotionDetector(motionDetector);
    mMotionController->setDataModel(mDataModel);
    mMotionController->setFrameWriter(frameWriter);
    mMotionController->setCameraName(mCameraName);
}

