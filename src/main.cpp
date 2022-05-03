
#include "CameraMotionDetection.h"
#include "MainWindow.h"


#include <iostream>
#include <QApplication>

#include <QTimer>
#include <thread>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    const auto w = new MainWindow;

    const auto cam1 = std::make_shared<CameraMotionDetection>("rtsp://localhost:61250/cam01", "outputcam1");
    const auto cam2 = std::make_shared<CameraMotionDetection>("rtsp://localhost:61250/cam02", "outputcam2");
    const auto cam3 = std::make_shared<CameraMotionDetection>("rtsp://localhost:61250/cam01", "outputcam3");

    w->addCameraMotionDetection(cam1);
    w->addCameraMotionDetection(cam2);
    w->addCameraMotionDetection(cam3);

    cam1->startMotionDetection ();
    cam2->startMotionDetection ();

    w->show();





//    cam1.startMotionDetection();
//    cam2.startMotionDetection();

//    QTimer timer;
//    timer.setInterval(5000);
//    timer.setSingleShot(true);
//    QObject::connect(&timer, &QTimer::timeout, &cam1, &CameraMotionDetection::stopMotionDetection);
//    QObject::connect(&timer, &QTimer::timeout, &cam2, &CameraMotionDetection::stopMotionDetection);
//    QObject::connect(&timer, &QTimer::timeout, [=](){qApp->exit();});

//    timer.start ();
    return a.exec();















//    const auto frameDataModel = std::make_shared<FrameDataModel>();
//    const auto frameAcquisitor = std::make_shared<FrameAcquisitor>(frameDataModel);
//    const auto frameWriter = std::make_shared<FrameWriter>();
//    frameWriter->setDataModel(frameDataModel);
//    const auto motionDetector = std::make_shared<MockMotionDetector<Mat>>();
//    motionDetector->initializeMotionVector();
//    const auto motionController = std::make_shared<MotionController>();
//    motionController->setMotionDetector(motionDetector);
//    motionController->setDataModel(frameDataModel);
//    motionController->setFrameWriter(frameWriter);

//    auto letThreadsRun = true;
//    std::thread threadFrameAcquisition (&FrameAcquisitor::runAcquisition, frameAcquisitor, &letThreadsRun);
//    std::thread threadMotionController (&MotionController::startMotionDetection, motionController, &letThreadsRun);

//    while (true)
//    {
//        const auto frame = frameDataModel->getFrame(0);

//        if (!frame.empty())
//            imshow("RTSP stream", frame);

//        if (waitKey(1) == 27)
//            break;
//    }
//    letThreadsRun = false;
//    threadFrameAcquisition.join();
//    threadMotionController.join();

//    destroyAllWindows();

//    return 0;

}
