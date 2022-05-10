#include "MotionRecorderWidget.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include <QTimer>


MotionRecorderWidget::MotionRecorderWidget(std::shared_ptr<CV_MotionRecorderManager> motionRecorderManager, QWidget *parent)
    : QWidget(parent), mMotionRecorderManager(motionRecorderManager)
{
    setFixedHeight(120);
    initResources ();
    setLayout(buildLayout ());

    mIconBlinkTimer = new QTimer (this);
    mIconBlinkTimer->setInterval(500);
    connect (mIconBlinkTimer, &QTimer::timeout, this, &MotionRecorderWidget::updateCamRecordingIcon);

    onMotDetStatusChanged = [=](bool isMotionDetectionRunning){
        if (isMotionDetectionRunning)
        {
            mStatusDetectionButton->setIcon(mRecordButtonIcon.pixmap(25,25,QIcon::Active));
            mCamRecordingLabel->setPixmap(mRecordingStatusIcon.pixmap(30, 30, QIcon::Normal));
            mIconBlinkTimer->start ();
        }
        else
        {
            mStatusDetectionButton->setIcon(mRecordButtonIcon.pixmap(25,25,QIcon::Normal));
            mCamRecordingLabel->setPixmap(mRecordingStatusIcon.pixmap(30, 30, QIcon::Disabled));
            mIconBlinkTimer->stop ();
        }
    };
    mMotionRecorderManager->setCallbackFunctionOnStatusChange(onMotDetStatusChanged);
    mIconBlinkTimer->start();
}

MotionRecorderWidget::~MotionRecorderWidget()
{
    mIconBlinkTimer->stop();

    if (mMotionRecorderManager->isMotionDetectionRunning())
    {
        mMotionRecorderManager->stopMotionDetection();
    }
}

void MotionRecorderWidget::onSecondsAfterMotionFinishesChanged(int seconds)
{
    mMotionRecorderManager->setSecondsAfterMotionFinishes(seconds);
}

void MotionRecorderWidget::showEvent(QShowEvent *event)
{
    //force to set the icons to its right state
    onMotDetStatusChanged (mMotionRecorderManager->isMotionDetectionRunning());
    QWidget::showEvent(event);
}

QLayout *MotionRecorderWidget::buildLayout()
{
    const auto layout = new QHBoxLayout;

    const auto cameraStreamLabel = new QLabel (QString::fromStdString(mMotionRecorderManager->getSourceStreamPath()));

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
    connect(mStatusDetectionButton, &QPushButton::clicked, this, &MotionRecorderWidget::onStatusDetectionButtonClicked);

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

void MotionRecorderWidget::updateCamRecordingIcon()
{
    QPixmap currentFrame;
    if (mMotionRecorderManager->getCurrentFrame(currentFrame, mFrameLabel->size ()))
        mFrameLabel->setPixmap(currentFrame);

    mIconBlinkVisible = !mIconBlinkVisible;
    if (mMotionRecorderManager->isRecordingVideo())
    {
        if (mIconBlinkVisible)
            mCamRecordingLabel->setPixmap(mRecordingStatusIcon.pixmap(30, 30, QIcon::Active));
        else
            mCamRecordingLabel->setPixmap(mRecordingStatusIcon.pixmap(30, 30, QIcon::Selected));
    }
    else
    {
        mCamRecordingLabel->setPixmap(mRecordingStatusIcon.pixmap(30, 30, QIcon::Normal));
    }
}

void MotionRecorderWidget::onStatusDetectionButtonClicked()
{
    const auto isMotionDetectionEnabled = mMotionRecorderManager->isMotionDetectionRunning();
    if (isMotionDetectionEnabled)
        mMotionRecorderManager->stopMotionDetection();
    else
        mMotionRecorderManager->startMotionDetection();
}

void MotionRecorderWidget::initResources()
{
    mRecordingStatusIcon.addFile("://cam_rec", QSize(), QIcon::Active);
    mRecordingStatusIcon.addFile("://cam_rec_blink", QSize(), QIcon::Selected);
    mRecordingStatusIcon.addFile("://cam_no_motion", QSize(), QIcon::Normal);
    mRecordingStatusIcon.addFile("://cam_off", QSize(), QIcon::Disabled);

    mRecordButtonIcon.addFile("://record", QSize(), QIcon::Normal);
    mRecordButtonIcon.addFile("://stop", QSize(), QIcon::Active);
}
