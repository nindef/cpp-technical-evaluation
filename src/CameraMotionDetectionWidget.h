#pragma once

#include "CameraMotionDetection.h"

#include <QWidget>
#include <QIcon>

class QLabel;
class QPushButton;
class QTimer;

class CameraMotionDetectionWidget : public QWidget
{
    Q_OBJECT

public:
    CameraMotionDetectionWidget(std::shared_ptr<CameraMotionDetection> motionDetection, QWidget *parent = nullptr);
    ~CameraMotionDetectionWidget ();

    void onSecondsAfterMotionFinishesChanged (int seconds);

protected:
    void showEvent(QShowEvent *event) override;

private:
    std::shared_ptr<CameraMotionDetection> mCameraMotionDetection;
    QLabel* mCamRecordingLabel = nullptr;
    QLabel* mFrameLabel = nullptr;
    bool mIconBlinkVisible = false;
    QIcon mMotionDetIcon;
    QIcon mStatusRecordingIcon;
    QPushButton* mStatusDetectionButton = nullptr;
    QTimer* mIconBlinkTimer = nullptr;

    QLayout *buildLayout();

    void updateCamRecordingIcon ();
    void onStatusDetectionButtonClicked ();
    std::function<void(bool)> onMotDetStatusChanged;
//    void setIconsRecordingStatus (bool isMotionDetectionEnabled);
};
