#pragma once

#include <QMainWindow>

class QVBoxLayout;
class QMediaPlayer;
class CameraMotionDetection;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void addCameraMotionDetection(std::shared_ptr<CameraMotionDetection> motionDetection);

private:
    QVBoxLayout* mGlobalLayout;

    QWidget *buildCentralWidget();
    void onConfigButtonClicked ();

signals:
    void secondsAfterMotionFinishesChanged(int seconds);
};
