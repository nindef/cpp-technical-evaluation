#pragma once

#include <iostream>
#include <thread>

#include <QPixmap>

class FrameDataModel;
class MotionController;
class FrameAcquisitor;

class CameraMotionDetection
{
public:
    CameraMotionDetection (std::string sourceStreamPath, std::string outputVideoBaseName);
    ~CameraMotionDetection () {};

    void startMotionDetection ();
    void stopMotionDetection();

    void setCallbackFunctionOnStatusChange (std::function<void(bool)> callbackFunction);

    void setSecondsAfterMotionFinishes (int seconds);

    const std::string& getSourceStreamPath() const;
    bool isMotionDetected () const;
    bool isMotionDetectionRunning () const;
    bool getCurrentFrame (QPixmap& pixmap, QSize size);

private:
    bool mThreadRunning = false;
    bool mMotionDetected = false;
    int mSecondsAfterMotionFinishes = 2;
    std::string mSourceStreamPath;
    std::thread * mThreadFrameAcquisition;
    std::thread * mThreadMotionController;
    std::function<void (bool)> mCallbackFunction;


    std::shared_ptr<FrameDataModel> mDataModel;
    std::shared_ptr<FrameAcquisitor> mFrameAcquisitor;
    std::shared_ptr<MotionController> mMotionController;

};
