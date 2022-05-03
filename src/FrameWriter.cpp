#include "FrameWriter.h"

#include "FrameDataModel.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs_c.h"
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QApplication>

FrameWriter::FrameWriter()
{
    mVideoWriter = std::make_shared<VideoWriter>();
}

void FrameWriter::setOutputVideoBaseName(const std::string &outputVideoBaseName)
{
    mOutputVideoBaseName = outputVideoBaseName;
}

void FrameWriter::setDataModel (std::shared_ptr<FrameDataModel> model)
{
    mDataModel = model;
}

void FrameWriter::writeFrame(Mat frame)
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
        mVideoWriter->open(videosFolder.toStdString() + finalOutputName, mSourceConfig.fourcc, mSourceConfig.fps, mSourceConfig.size);
    }

    mVideoWriter->write(frame);
}

double FrameWriter::getFPS() const
{
    return mSourceConfig.fps;
}

void FrameWriter::closeFile()
{
    mVideoWriter->release();
}
