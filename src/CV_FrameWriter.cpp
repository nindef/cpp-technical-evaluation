#include "CV_FrameWriter.h"

#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QApplication>


CV_FrameWriter::CV_FrameWriter()
{
    mVideoWriter = std::make_shared<VideoWriter>();
}

void CV_FrameWriter::writeFrame(std::shared_ptr<IFrameType> frame)
{
    assert(mDataModel != nullptr);

    if (!mVideoWriter->isOpened())
    {
        const auto finalOutputName = mOutputVideoBaseName + "_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss").toStdString() + ".mp4";
        auto videosFolder = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
        videosFolder = videosFolder + "/" + QApplication::applicationName() + "/";

        QDir outputDir (videosFolder);
        if (!outputDir.exists())
            outputDir.mkdir(videosFolder);

        mSourceConfig = mDataModel->getSrcVideoConfig();
        const auto fourcc = VideoWriter::fourcc('a','v','c','1');
        mVideoWriter->open(videosFolder.toStdString() + finalOutputName, fourcc, mSourceConfig.fps, Size(mSourceConfig.width, mSourceConfig.height));
    }

    const auto cvFrame = std::dynamic_pointer_cast<CVFrameWrapper>(frame);
    mVideoWriter->write(*cvFrame->getFrame());
}

void CV_FrameWriter::closeFile()
{
    mVideoWriter->release();
}
