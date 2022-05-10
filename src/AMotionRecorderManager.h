#pragma once

#include "FrameDataModel.h"

#include <iostream>
#include <thread>

#include <QPixmap>

class FrameDataModel;
class MotionController;
class AFrameAcquisitor;

class AMotionRecorderManager
{
public:
    AMotionRecorderManager (std::string sourceStreamPath, std::string outputVideoBaseName);
    ~AMotionRecorderManager () {};

    void startMotionDetection ();
    void stopMotionDetection();

    void setCallbackFunctionOnStatusChange (std::function<void(bool)> callbackFunction);

    void setSecondsAfterMotionFinishes (int seconds);

    const std::string& getSourceStreamPath() const;
    bool isRecordingVideo () const;
    bool isMotionDetectionRunning () const;
    bool getCurrentFrame (QPixmap& pixmap, QSize size);

protected:
    bool mMembersBuilt = false;
    bool mThreadRunning = false;
    bool mIsRecording = false;
    int mSecondsAfterMotionFinishes = 2;
    std::string mSourceStreamPath;
    std::string mOutputVideoBaseName;
    std::thread * mThreadFrameAcquisition;
    std::thread * mThreadMotionController;
    std::function<void (bool)> mCallbackFunction;

    std::shared_ptr<FrameDataModel> mDataModel;
    std::shared_ptr<AFrameAcquisitor> mFrameAcquisitor;
    std::shared_ptr<MotionController> mMotionController;

    virtual void buildMembers () = 0;

private:
    void build ();
};
