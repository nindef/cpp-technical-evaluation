#pragma once

#include "MotionRecorderManager.h"

#include <QWidget>
#include <QIcon>

class QLabel;
class QPushButton;
class QTimer;

class MotionRecorderWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief MotionRecorderWidget Constructor for this widget.
     * Sets the layout, starts a timer to blink the icon when recording
     * and also sets the callback function called by the MotionRecorderManager object
     * when motion detection status changes.
     * @param motionRecorderManager The MotionRecorderManager object.
     * @param parent The parent QWidget.
     */
    MotionRecorderWidget(std::shared_ptr<MotionRecorderManager> motionRecorderManager, QWidget *parent = nullptr);

    /**
     * @brief ~MotionRecorderWidget Default destructor.
     * Stops the blinking timer and the motion detection system (if it's running)
     */
    ~MotionRecorderWidget ();

    /**
     * @brief onSecondsAfterMotionFinishesChanged Method called when the user changes
     * the seconds to keep recording video after the motion detection finishes.
     * @param seconds The seconds to keep recording after the motion detection finishes.
     */
    void onSecondsAfterMotionFinishesChanged (int seconds);

protected:
    /**
     * @brief showEvent Override for the showEvent, to set the icons state
     * @param event The QShowEvent
     */
    void showEvent(QShowEvent *event) override;

private:
    std::shared_ptr<MotionRecorderManager> mMotionRecorderManager;
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
    void initResources ();
};
