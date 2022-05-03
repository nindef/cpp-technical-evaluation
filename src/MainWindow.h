#pragma once

#include <QMainWindow>

class QVBoxLayout;
class QMediaPlayer;
class MotionRecorderManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow Constructor. Builds the layout of this window
     * @param parent The parent widget
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief addMotionRecorder Adds a camera to this window. This method creates a MotionRecorderWidget
     * and inserts it into this window layout.
     * @param motionDetection The MotionRecorderManager object
     */
    void addMotionRecorder(std::shared_ptr<MotionRecorderManager> motionDetection);

private:
    QVBoxLayout* mGlobalLayout;

    QWidget *buildCentralWidget();
    void onConfigButtonClicked ();

signals:
    /**
     * @brief secondsAfterMotionFinishesChanged Signal raised when user changes the seconds to record
     * after the system detects no motion
     * @param seconds Seconds to keep recording after the system detects no motion.
     */
    void secondsAfterMotionFinishesChanged(int seconds);
};
