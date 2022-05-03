#pragma once

#include <iostream>
#include "IFrameDataModel.h"

template<typename FT, typename VC>
class IFrameAcquisitor
{
public:
    /**
     * @brief ~IFrameAcquisitor Default destructor. Releases the VideoCapture object.
     */
    ~IFrameAcquisitor()
    {
        if (mVideoCapture != nullptr)
            mVideoCapture->release();
    }

    /**
     * @brief setDataModel Sets the acquired frames model.
     * @param model The acquired frames data model shared pointer
     */
    void setDataModel(std::shared_ptr<IFrameDataModel<FT>> model)
    {
        mDataModel = model;
    }

    /**
     * @brief configure This method is used to configure the VideoCapture object.
     */
    virtual void configure (std::string sourceStreamPath) = 0;

    /**
     * @brief runAcquisition Starts the frame acquisition process.
     * @param [in] threadRunning This flag tells when this process has to stop
     */
    virtual void runAcquisition (bool* threadRunning) = 0;

protected:
    bool *mThreadRunning = nullptr;
    std::shared_ptr<VC> mVideoCapture;
    std::shared_ptr<IFrameDataModel<FT>> mDataModel;
};
