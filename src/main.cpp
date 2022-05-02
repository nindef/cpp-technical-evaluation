#include "FrameAcquisitor.h"
#include "FrameDataModel.h"
#include "FrameWriter.h"
#include "MotionController.h"
#include "MainWindow.h"


#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <QApplication>

#include <QDateTime>
#include <thread>

using namespace cv;

int main(int argc, char *argv[])
{

    const auto frameDataModel = std::make_shared<FrameDataModel>();
    const auto frameAcquisitor = std::make_shared<FrameAcquisitor>(frameDataModel);
    const auto frameWriter = std::make_shared<FrameWriter>();
    frameWriter->setDataModel(frameDataModel);
    const auto motionDetector = std::make_shared<MockMotionDetector<Mat>>();
    motionDetector->initializeMotionVector();
    const auto motionController = std::make_shared<MotionController>();
    motionController->setMotionDetector(motionDetector);
    motionController->setDataModel(frameDataModel);
    motionController->setFrameWriter(frameWriter);

    auto letThreadsRun = true;
    std::thread threadFrameAcquisition (&FrameAcquisitor::runAcquisition, frameAcquisitor, &letThreadsRun);
    std::thread threadMotionController (&MotionController::startMotionDetection, motionController, &letThreadsRun);

    while (true)
    {
        const auto frame = frameDataModel->getFrame(0);

        if (!frame.empty())
            imshow("RTSP stream", frame);

        if (waitKey(1) == 27)
            break;
    }
    letThreadsRun = false;
    threadFrameAcquisition.join();
    threadMotionController.join();





//    const std::string RTSP_URL = "rtsp://localhost:61250/cam01";
//    const auto frameAcquisitor = std::make_shared<FrameAcquisitor>(RTSP_URL, CAP_FFMPEG);
//    const auto motionController = std::make_shared<MotionController>(frameAcquisitor);
//    while (true) {
//        std::thread t1 (&MotionController::play, motionController);
//        t1.join();

//        imshow("RTSP stream", motionController->getFramePlayed());

//        if (waitKey(1) == 27)
//            break;
//    }

////    QApplication a(argc, argv);
////    MainWindow w;
////    w.show();
////    return a.exec();

//    const std::string RTSP_URL = "rtsp://localhost:61250/cam01";

//#if WIN32
//    _putenv_s("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;udp");
//#else
//    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;udp", 1);
//#endif

//    Mat frame;
//    Mat prevFrame;
//    VideoCapture cap(RTSP_URL, CAP_FFMPEG);

//    if (!cap.isOpened()) {
//        std::cout << "Cannot open RTSP stream" << std::endl;

//        return -1;
//    }

//    const auto fourcc = cap.get(CAP_PROP_FOURCC);
//    const auto fps = cap.get(CAP_PROP_FPS);
//    const auto size = Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));

//    VideoWriter videoWriter ("output.mp4", fourcc, fps, size);

//    MockMotionDetector<Mat> motionDetector;

//    while (true) {
//        cap >> frame;

//        putText(frame,
//                QDateTime::currentDateTime ().toString ("yyyy/MM/dd hh:mm:ss.zzz").toStdString(),
//                cv::Point(20,40), // Coordinates (Bottom-left corner of the text string in the image)
//                cv::FONT_ITALIC, // Font
//                1.0, // Scale. 2.0 = 2x bigger
//                cv::Scalar(255,255,255), // BGR Color
//                1, // Line Thickness (Optional)
//                cv:: LINE_AA);

//        imshow("RTSP stream", frame);

//        if(!prevFrame.empty() && motionDetector.detect(prevFrame, frame) == Motion::MOTION)
//        {
////            std::cout << "recording" << std::endl;
//            videoWriter.write(frame);
//        }
////        else
////            std::cout << "no recording" << std::endl;

//        if (waitKey(1) == 27) {
//            break;
//        }

//        prevFrame = std::move(frame);
//    }

//    cap.release();
//    videoWriter.release ();
    destroyAllWindows();

    return 0;
}
