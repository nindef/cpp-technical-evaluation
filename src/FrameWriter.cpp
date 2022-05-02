#include "FrameWriter.h"

#include "FrameDataModel.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs/imgcodecs_c.h"
#include <QDateTime>

FrameWriter::FrameWriter()
{
}

FrameWriter::~FrameWriter()
{
    mVideoWriter->release();
}

void FrameWriter::setDataModel (std::shared_ptr<FrameDataModel> model)
{
    mDataModel = model;
}

void FrameWriter::writeFrame(Mat frame)
{
    assert(mDataModel != nullptr);

    if (mVideoWriter == nullptr)
    {
        const auto sourceConfig = mDataModel->getSrcVideoConfig();
        mVideoWriter = std::make_shared<VideoWriter>("output.mp4", sourceConfig.fourcc, sourceConfig.fps, sourceConfig.size);
    }

    mVideoWriter->write(frame);
}
