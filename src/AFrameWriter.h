#pragma once

#include "FrameDataModel.h"
#include <iostream>

class AFrameWriter
{
public:
    /**
     * @brief FrameWriter Default constructor
     */
    AFrameWriter () = default;

    /**
     * @brief setOutputVideoBaseName Sets the output file name. The extension will be mp4 and
     * the location will be the user Video standard location.
     * @param outputVideoBaseName The base name of the output video file.
     */
    void setOutputVideoBaseName (const std::string& outputVideoBaseName)
    {
        mOutputVideoBaseName = outputVideoBaseName;
    }

    /**
     * @brief setDataModel Sets the acquired frames model.
     * @param model The acquired frames data model shared pointer
     */
    void setDataModel(std::shared_ptr<FrameDataModel> model)
    {
        mDataModel = model;
    }

    /**
     * @brief writeFrame Writes the frame into the video by using the VideoWriter.
     * If the VideoWriter hasn't opened yet the video file, this method opens it
     * @param frame The frame to write into the video.
     */
    virtual void writeFrame(std::shared_ptr<IFrameType> frame) = 0;

    /**
     * @brief getFPS Gets the output FPS (that actually are the same than the input video stream)
     * @return The frames per second set for the output video file.
     */
    double getFPS () const
    {
        return mSourceConfig.fps;
    }

    /**
     * @brief closeFile Closes this file (so, releases the VideoWriter)
     */
    virtual void closeFile () = 0;

protected:
    bool *mRunning = nullptr;
    std::string mOutputVideoBaseName;
    std::shared_ptr<FrameDataModel> mDataModel;
    typename FrameDataModel::VideoConfig mSourceConfig;
};
