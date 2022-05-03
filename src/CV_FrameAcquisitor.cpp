#include "CV_FrameAcquisitor.h"

#include "IFrameDataModel.h"


void CV_FrameAcquisitor::configure(std::string sourceStreamPath)
{
    mVideoCapture = std::make_shared<VideoCapture>(std::move(sourceStreamPath), VideoCaptureAPIs::CAP_ANY);

#if WIN32
    _putenv_s("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;udp");
#else
    setenv("OPENCV_FFMPEG_CAPTURE_OPTIONS", "rtsp_transport;udp", 1);
#endif
}

void CV_FrameAcquisitor::runAcquisition(bool* threadRunning)
{
    if (mVideoCapture != nullptr && mVideoCapture->isOpened())
    {
        const auto fourcc = mVideoCapture->get(CAP_PROP_FOURCC);
        const auto fps = mVideoCapture->get(CAP_PROP_FPS);
        const auto size = Size(mVideoCapture->get(CAP_PROP_FRAME_WIDTH), mVideoCapture->get(CAP_PROP_FRAME_HEIGHT));
        IFrameDataModel<Mat>::VideoConfig videoConfig (fourcc, fps, size.width, size.height);
        mDataModel->setSrcVideoConfig(videoConfig);

        while(*threadRunning)
        {
            Mat frame;
            if (mVideoCapture->read(frame))
                mDataModel->storeFrame(frame);
        }
    }
}

