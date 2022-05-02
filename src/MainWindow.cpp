#include "MainWindow.h"

#include <QGridLayout>
#include <QMediaPlayer>
#include <QMediaRecorder>
#include <QNetworkRequest>
#include <QVideoWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Test");

    const auto layout = new QHBoxLayout;

    const auto _vw1 = new QVideoWidget;
    const auto _player1 = new QMediaPlayer;
    layout->addWidget(_vw1);

    QWidget *win = new QWidget();
    win->setLayout(layout);
    setCentralWidget(win);

    _player1->setVideoOutput(_vw1);
    const QUrl url1 = QUrl("rtsp://localhost:61250/cam01");
    _player1->setMedia(url1);

    const auto mediaRecorder = new QMediaRecorder(_player1);
    mediaRecorder->setOutputLocation(QUrl("recordedOutput.mp4"));

    _player1->play();
    mediaRecorder->record();
}

MainWindow::~MainWindow()
{
}

