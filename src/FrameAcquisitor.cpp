#include "FrameAcquisitor.h"

#include "FrameDataModel.h"


FrameAcquisitor::FrameAcquisitor()
{
}

FrameAcquisitor::~FrameAcquisitor()
{
    if (mVideoCapture != nullptr)
        mVideoCapture->release();
}

void FrameAcquisitor::setDataModel (std::shared_ptr<FrameDataModel> model)
{
    mDataModel = model;
}

void FrameAcquisitor::configure(std::string sourceStreamPath, VideoCaptureAPIs videoCaptureAPI)
{
    mVideoCapture = std::make_shared<VideoCapture>(std::move(sourceStreamPath), videoCaptureAPI);

#if WIN32
    _putenv_s("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;udp");
#else
    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;udp", 1);
#endif
}

void FrameAcquisitor::runAcquisition(bool* frameControlRunning)
{
    mThreadRunning = frameControlRunning;
    if (mVideoCapture != nullptr && mVideoCapture->isOpened())
    {
        const auto fourcc = mVideoCapture->get(CAP_PROP_FOURCC);
        const auto fps = mVideoCapture->get(CAP_PROP_FPS);
        const auto size = Size(mVideoCapture->get(CAP_PROP_FRAME_WIDTH), mVideoCapture->get(CAP_PROP_FRAME_HEIGHT));
        mDataModel->setSrcVideoConfig(fourcc, fps, size);

        while(*mThreadRunning)
        {
            Mat frame;
            if (mVideoCapture->read(frame))
                mDataModel->storeFrame(frame);
        }
    }
}

