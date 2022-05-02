#include "FrameController.h"

#include "FrameDataModel.h"
#include "FrameAcquisitor.h"

FrameController::FrameController(std::shared_ptr<FrameDataModel> model) : mDataModel (model)
{
    const std::string RTSP_URL = "rtsp://localhost:61250/cam01";
    mFrameAcquisitor = std::make_shared<FrameAcquisitor>(RTSP_URL, CAP_FFMPEG);
}

void FrameController::runFrameControl(bool* frameControlRunning)
{
    mThreadRunning = frameControlRunning;
    getAndStoreFrame ();
}

void FrameController::getAndStoreFrame()
{
    while(*mThreadRunning)
    {
//        const auto frame = mFrameAcquisitor->getFrame();
//        mDataModel->storeFrame(frame);
    }
}
