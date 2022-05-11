
#include "CV_MotionRecorderManager.h"
#include "MainWindow.h"

#ifdef WIN32
#include <conio.h>
#else
#include <curses.h>
#endif

#include <future>
#include <iostream>
#include <QApplication>

#include <QDebug>
#include <QTimer>

void printHelp ()
{
    std::cout << "Motion Recorder HELP" << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << "Possible arguments for this application are the following:" << std::endl;
    std::cout << "\t --no-gui \t\tThis application does not show its GUI and runs on command line." << std::endl;
    std::cout << "\t --secs-later <SECS> \tTo define how many seconds this application records after motion detection finishes." << std::endl;
}

int main(int argc, char *argv[])
{
    auto isConsoleMode = false;

    auto secs_later = 2;

    for (auto i = 0; i < argc; i++)
    {
        if (std::strcmp(argv[i],"--help") == 0)
        {
            printHelp();
            return 0;
        }

        isConsoleMode |= (std::strcmp(argv[i],"--no-gui") == 0);

        if (std::strcmp(argv[i],"--secs-later") == 0)
        {
            if (argc > i+1)
                secs_later = std::strtol (argv[i + 1], nullptr, 10);
            else
            {
                std::cout << "ERROR: Seconds parameter missing" << std::endl << std::endl;
                printHelp ();
                return 0;
            }
        }
    }

    const auto cam1 = std::make_shared<CV_MotionRecorderManager>("rtsp://localhost:61250/cam01", "cam1");
    const auto cam2 = std::make_shared<CV_MotionRecorderManager>("rtsp://localhost:61250/cam02", "cam2");
    const auto cam3 = std::make_shared<CV_MotionRecorderManager>("rtsp://localhost:61250/cam01", "cam3");

    cam1->setSecondsAfterMotionFinishes(secs_later);
    cam2->setSecondsAfterMotionFinishes(secs_later);
    cam3->setSecondsAfterMotionFinishes(secs_later);

    auto retCode = 0;

    if (!isConsoleMode)
    {
        QApplication a (argc, argv);

        const auto w = new MainWindow;
        w->addMotionRecorder(cam1);
        w->addMotionRecorder(cam2);
        w->addMotionRecorder(cam3);
        w->show();

        cam1->startMotionDetection ();
        cam2->startMotionDetection ();
        retCode = a.exec();
        cam1->stopMotionDetection ();
        cam2->stopMotionDetection ();
    }
    else
    {
        QCoreApplication a (argc, argv);

        cam1->startMotionDetection ();
        cam2->startMotionDetection ();

        bool exitFlag = false;

        auto keyCatcher = std::async(std::launch::async, [&exitFlag]
        {
            while (!exitFlag)
            {

#ifdef WIN32
                const auto ch = _getch ();
#else
                const auto ch = getch ();
#endif
                exitFlag = (ch == 27); //ESC key
            }
        });

        QTimer exitTimer;
        exitTimer.setInterval(500);
        exitTimer.setSingleShot(false);

        QObject::connect(&exitTimer, &QTimer::timeout, [&a,&exitFlag]
        {
            if (exitFlag)
                a.quit();
        });

        exitTimer.start();

        std::cout << "Motion detection started. Press ESC to quit..." << std::endl;
        retCode = a.exec();
        std::cout.flush();
        keyCatcher.wait();

        cam1->stopMotionDetection ();
        cam2->stopMotionDetection ();
    }

    return retCode;

}
