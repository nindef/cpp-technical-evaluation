#pragma once

#include "IFrameDataModel.h"

#include <iostream>
#include <thread>

#include <QPixmap>

namespace cv { class Mat; }
class CV_FrameDataModel;
class CV_MotionController;
class CV_FrameAcquisitor;

class MotionRecorderManager
{
public:
    MotionRecorderManager (std::string sourceStreamPath, std::string outputVideoBaseName);
    ~MotionRecorderManager () {};

    void startMotionDetection ();
    void stopMotionDetection();

    void setCallbackFunctionOnStatusChange (std::function<void(bool)> callbackFunction);

    void setSecondsAfterMotionFinishes (int seconds);

    const std::string& getSourceStreamPath() const;
    bool isRecordingVideo () const;
    bool isMotionDetectionRunning () const;
    bool getCurrentFrame (QPixmap& pixmap, QSize size);

private:
    bool mThreadRunning = false;
    bool mIsRecording = false;
    int mSecondsAfterMotionFinishes = 2;
    std::string mSourceStreamPath;
    std::thread * mThreadFrameAcquisition;
    std::thread * mThreadMotionController;
    std::function<void (bool)> mCallbackFunction;


    std::shared_ptr<CV_FrameDataModel> mDataModel;
    std::shared_ptr<CV_FrameAcquisitor> mFrameAcquisitor;
    std::shared_ptr<CV_MotionController> mMotionController;

};
