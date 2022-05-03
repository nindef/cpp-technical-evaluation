#pragma once

#include "IFrameDataModel.h"
#include <iostream>


template<typename FD, typename VW>
class IFrameWriter
{
public:
    /**
     * @brief FrameWriter Default constructor
     */
    IFrameWriter ()
    {
        mVideoWriter = std::make_shared<VW>();
    }

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
    void setDataModel(std::shared_ptr<IFrameDataModel<FD>> model)
    {
        mDataModel = model;
    }

    /**
     * @brief writeFrame Writes the frame into the video by using the VideoWriter.
     * If the VideoWriter hasn't opened yet the video file, this method opens it
     * @param frame The frame to write into the video.
     */
    virtual void writeFrame(FD frame) = 0;

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
    std::shared_ptr<IFrameDataModel<FD>> mDataModel;
    std::shared_ptr<VW> mVideoWriter;
    typename IFrameDataModel<FD>::VideoConfig mSourceConfig;
};
