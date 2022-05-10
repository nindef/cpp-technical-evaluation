#pragma once

#include <iostream>
#include "FrameDataModel.h"

class AFrameAcquisitor
{
public:
    /**
     * @brief ~IFrameAcquisitor Default destructor. Releases the VideoCapture object.
     */
    ~AFrameAcquisitor() = default;

    /**
     * @brief setDataModel Sets the acquired frames model.
     * @param model The acquired frames data model shared pointer
     */
    void setDataModel(std::shared_ptr<FrameDataModel> model)
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
    std::shared_ptr<FrameDataModel> mDataModel;
};
