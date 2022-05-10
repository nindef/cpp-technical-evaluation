#include "CV_FrameAcquisitor.h"

#include "FrameDataModel.h"
#include "FrameWrapper.h"


CV_FrameAcquisitor::CV_FrameAcquisitor()
{
    mVideoCapture = std::make_shared<VideoCapture>();
}

CV_FrameAcquisitor::~CV_FrameAcquisitor()
{
    if (mVideoCapture != nullptr)
        mVideoCapture->release();
}

void CV_FrameAcquisitor::configure(std::string sourceStreamPath)
{
    mVideoCapture->open(std::move(sourceStreamPath), VideoCaptureAPIs::CAP_ANY);

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
        FrameDataModel::VideoConfig videoConfig (fourcc, fps, size.width, size.height);
        mDataModel->setSrcVideoConfig(videoConfig);
    }
}

void CV_FrameAcquisitor::runAcquisition(bool* threadRunning)
{
    if (mVideoCapture != nullptr && mVideoCapture->isOpened())
    {
        while(*threadRunning)
        {
            Mat mat;
            if (mVideoCapture->read(mat))
            {
                const auto sharedFrame = std::make_shared<CVFrameWrapper>(std::make_shared<Mat>(mat));
                mDataModel->storeFrame(sharedFrame);
            }
        }
    }
}

