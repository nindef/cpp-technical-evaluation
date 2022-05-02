#include "FrameAcquisitor.h"

#include "FrameDataModel.h"


FrameAcquisitor::FrameAcquisitor(std::shared_ptr<FrameDataModel> model) : mDataModel(model)
{
    const std::string sourceStreamPath = "rtsp://localhost:61250/cam01";
    mVideoCapture = std::make_shared<VideoCapture>(sourceStreamPath, CAP_FFMPEG);

#if WIN32
    _putenv_s("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;udp");
#else
    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;udp", 1);
#endif
    if (mVideoCapture->isOpened())
    {
        const auto fourcc = mVideoCapture->get(CAP_PROP_FOURCC);
        const auto fps = mVideoCapture->get(CAP_PROP_FPS);
        const auto size = Size(mVideoCapture->get(CAP_PROP_FRAME_WIDTH), mVideoCapture->get(CAP_PROP_FRAME_HEIGHT));
        mDataModel->setSrcVideoConfig(fourcc, fps, size);
    }
    else
        mVideoCapture = nullptr;
}

FrameAcquisitor::~FrameAcquisitor()
{
    if (mVideoCapture != nullptr)
        mVideoCapture->release();
}

void FrameAcquisitor::runAcquisition(bool* frameControlRunning)
{
    mThreadRunning = frameControlRunning;
    if (mVideoCapture != nullptr)
    {
        Mat frame;
        while(*mThreadRunning)
        {
            *mVideoCapture >> frame;
            if (!frame.empty())
                mDataModel->storeFrame(frame);
        }
    }
}

