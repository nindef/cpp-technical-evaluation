
#include "CV_MotionRecorderManager.h"
#include "MainWindow.h"


#include <iostream>
#include <QApplication>

#include <QTimer>
#include <thread>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    const auto w = new MainWindow;

    const auto cam1 = std::make_shared<CV_MotionRecorderManager>("rtsp://localhost:61250/cam01", "outputcam1");
    const auto cam2 = std::make_shared<CV_MotionRecorderManager>("rtsp://localhost:61250/cam02", "outputcam2");
    const auto cam3 = std::make_shared<CV_MotionRecorderManager>("rtsp://localhost:61250/cam01", "outputcam3");

    w->addMotionRecorder(cam1);
    w->addMotionRecorder(cam2);
    w->addMotionRecorder(cam3);

    cam1->startMotionDetection ();
    cam2->startMotionDetection ();

    w->show();

    return a.exec();
}
