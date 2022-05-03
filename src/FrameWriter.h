#pragma once

#include "opencv2/videoio.hpp"
#include "FrameDataModel.h"
#include <iostream>

using namespace cv;

/**
 * @brief The FrameWriter class manages the process to write frames into a video file
 */
class FrameWriter
{
public:
    /**
     * @brief FrameWriter Default constructor
     */
    FrameWriter ();

    /**
     * @brief setOutputVideoBaseName Sets the output file name. The extension will be mp4 and
     * the location will be the user Video standard location.
     * @param outputVideoBaseName The base name of the output video file.
     */
    void setOutputVideoBaseName (const std::string& outputVideoBaseName);

    /**
     * @brief setDataModel Sets the acquired frames model.
     * @param model The acquired frames data model shared pointer
     */
    void setDataModel(std::shared_ptr<FrameDataModel> model);

    /**
     * @brief writeFrame Writes the frame into the video by using the VideoWriter.
     * If the VideoWriter hasn't opened yet the video file, this method opens it
     * @param frame The frame to write into the video.
     */
    void writeFrame(Mat frame);

    /**
     * @brief getFPS Gets the output FPS (that actually are the same than the input video stream)
     * @return The frames per second set for the output video file.
     */
    double getFPS () const;

    /**
     * @brief closeFile Closes this file (so, releases the VideoWriter)
     */
    void closeFile ();

private:
    bool *mRunning = nullptr;
    std::string mOutputVideoBaseName;
    std::shared_ptr<FrameDataModel> mDataModel;
    std::shared_ptr<VideoWriter> mVideoWriter;
    FrameDataModel::VideoConfig mSourceConfig;
};
