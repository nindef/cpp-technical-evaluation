#include "CameraMotionDetectionWidget.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QTimer>


CameraMotionDetectionWidget::CameraMotionDetectionWidget(std::shared_ptr<CameraMotionDetection> motionDetection, QWidget *parent)
    : QWidget(parent), mCameraMotionDetection(motionDetection)
{
    setFixedHeight(120);

    setLayout(buildLayout ());

    onMotDetStatusChanged = [=](bool isMotionDetectionRunning){
        if (isMotionDetectionRunning)
        {
            mStatusDetectionButton->setIcon(mStatusRecordingIcon.pixmap(25,25,QIcon::Active));
            mCamRecordingLabel->setPixmap(mMotionDetIcon.pixmap(30, 30, QIcon::Normal));
            mIconBlinkTimer->start ();
        }
        else
        {
            mStatusDetectionButton->setIcon(mStatusRecordingIcon.pixmap(25,25,QIcon::Normal));
            mCamRecordingLabel->setPixmap(mMotionDetIcon.pixmap(30, 30, QIcon::Disabled));
            mIconBlinkTimer->stop ();
        }
    };
    mCameraMotionDetection->setCallbackFunctionOnStatusChange(onMotDetStatusChanged);

    mIconBlinkTimer = new QTimer (this);
    mIconBlinkTimer->setInterval(500);
    connect (mIconBlinkTimer, &QTimer::timeout, this, &CameraMotionDetectionWidget::updateCamRecordingIcon);
    mIconBlinkTimer->start();
}

CameraMotionDetectionWidget::~CameraMotionDetectionWidget()
{
    mIconBlinkTimer->stop();

    if (mCameraMotionDetection->isMotionDetectionRunning())
    {
        mCameraMotionDetection->stopMotionDetection();
    }
}

void CameraMotionDetectionWidget::onSecondsAfterMotionFinishesChanged(int seconds)
{
    mCameraMotionDetection->setSecondsAfterMotionFinishes(seconds);
}

void CameraMotionDetectionWidget::showEvent(QShowEvent *event)
{
    mMotionDetIcon.addFile("://cam_rec", QSize(), QIcon::Active);
    mMotionDetIcon.addFile("://cam_rec_blink", QSize(), QIcon::Selected);
    mMotionDetIcon.addFile("://cam_no_motion", QSize(), QIcon::Normal);
    mMotionDetIcon.addFile("://cam_off", QSize(), QIcon::Disabled);

    mStatusRecordingIcon.addFile("://record", QSize(), QIcon::Normal);
    mStatusRecordingIcon.addFile("://stop", QSize(), QIcon::Active);

    //force to set the icons to its right state
    onMotDetStatusChanged (mCameraMotionDetection->isMotionDetectionRunning());
}

QLayout *CameraMotionDetectionWidget::buildLayout()
{
    const auto layout = new QHBoxLayout;

    const auto cameraStreamLabel = new QLabel (QString::fromStdString(mCameraMotionDetection->getSourceStreamPath()));

    mFrameLabel = new QLabel;
    mFrameLabel->setFixedWidth(150);

    const auto iconLabelContainer = new QWidget;
    iconLabelContainer->setFixedSize(50,50);
    iconLabelContainer->setContentsMargins(0,0,0,0);
    const auto iconLabelContainerLayout = new QHBoxLayout;
    iconLabelContainerLayout->setContentsMargins(0,0,0,0);
    iconLabelContainerLayout->setSpacing(0);
    mCamRecordingLabel = new QLabel;
    mCamRecordingLabel->setContentsMargins(0,0,0,0);
    iconLabelContainerLayout->addWidget(mCamRecordingLabel);
    iconLabelContainerLayout->setAlignment(mCamRecordingLabel, Qt::AlignCenter);
    iconLabelContainer->setLayout(iconLabelContainerLayout);

    mStatusDetectionButton = new QPushButton(this);
    mStatusDetectionButton->setToolTip("Stop motion detection");
    connect(mStatusDetectionButton, &QPushButton::clicked, this, &CameraMotionDetectionWidget::onStatusDetectionButtonClicked);

    layout->addWidget(mFrameLabel);
    layout->addWidget(cameraStreamLabel);
    layout->addWidget(iconLabelContainer);
    layout->addWidget(mStatusDetectionButton);

    layout->setAlignment(mFrameLabel, Qt::AlignRight);
    layout->setAlignment(cameraStreamLabel, Qt::AlignRight);
    layout->setAlignment(iconLabelContainer, Qt::AlignCenter);
    layout->setAlignment(mStatusDetectionButton, Qt::AlignCenter);
    layout->addStretch();

    return layout;
}

void CameraMotionDetectionWidget::updateCamRecordingIcon()
{
    QPixmap currentFrame;
    if (mCameraMotionDetection->getCurrentFrame(currentFrame, mFrameLabel->size ()))
    {
        mFrameLabel->setPixmap(currentFrame);
    }

    mIconBlinkVisible = !mIconBlinkVisible;
    if (mCameraMotionDetection->isMotionDetected())
    {
        if (mIconBlinkVisible)
            mCamRecordingLabel->setPixmap(mMotionDetIcon.pixmap(30, 30, QIcon::Active));
        else
            mCamRecordingLabel->setPixmap(mMotionDetIcon.pixmap(30, 30, QIcon::Selected));
    }
    else
    {
        mCamRecordingLabel->setPixmap(mMotionDetIcon.pixmap(30, 30, QIcon::Normal));
    }
}

void CameraMotionDetectionWidget::onStatusDetectionButtonClicked()
{
    const auto isMotionDetectionEnabled = mCameraMotionDetection->isMotionDetectionRunning();
    if (isMotionDetectionEnabled)
        mCameraMotionDetection->stopMotionDetection();
    else
        mCameraMotionDetection->startMotionDetection();

//    setIconsRecordingStatus(isMotionDetectionEnabled);
}

//void CameraMotionDetectionWidget::setIconsRecordingStatus(bool isMotionDetectionEnabled)
//{
//    if (isMotionDetectionEnabled)
//    {
//        mStatusDetectionButton->setIcon(mStatusRecordingIcon.pixmap(25,25,QIcon::Normal));
//        mCamRecordingLabel->setPixmap(mMotionDetIcon.pixmap(30, 30, QIcon::Disabled));
//        mIconBlinkTimer->stop ();
//    }
//    else
//    {
//        mStatusDetectionButton->setIcon(mStatusRecordingIcon.pixmap(25,25,QIcon::Active));
//        mCamRecordingLabel->setPixmap(mMotionDetIcon.pixmap(30, 30, QIcon::Normal));
//        mIconBlinkTimer->start ();
//    }
//}
