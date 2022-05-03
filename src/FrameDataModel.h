#pragma once

#include <QPixmap>
#include <deque>
#include <mutex>
#include <opencv2/core/mat.hpp>
using namespace cv;

/**
 * @brief The FrameDataModel class manages the acquired frames.
 * Even if it's not the job for a model to convert Mat frames to QPixmap, it also does it for convenience.
 */
class FrameDataModel
{
public:
    /**
     * @brief The SourceVideoConfig struct stores the basic information from a video.
     */
    struct VideoConfig{
        double fourcc = 0;
        double fps = 0;
        Size size = {0,0};

        VideoConfig (){}
        VideoConfig (double fourcc, double fps, Size size)
        {
            this->fourcc = fourcc;
            this->fps = fps;
            this->size = size;
        }
    };

    /**
     * @brief setSrcVideoConfig Sets the source video config to store it.
     * The VideoWriter will use this same configuration to set its parameters.
     * @param fourcc The FourCC code.
     * @param fps The frames per second.
     * @param size The frame size.
     */
    void setSrcVideoConfig (double fourcc, double fps, Size size);

    /**
     * @brief getSrcVideoConfig Gets the source video config, stored before.
     * @return The videoConfig for the source video.
     */
    FrameDataModel::VideoConfig& getSrcVideoConfig();

    /**
     * @brief storeFrame Stores an acquired frame (from FramAcquisitor) into this class deque
     * @param frame The frame to store
     */
    void storeFrame(Mat frame);

    /**
     * @brief getCurrentFramePixmap Gets the current frame (frame in first place in deque) as a QPixmap
     * @param pixmap The destiny QPixmap
     * @param dstSize The desired size for the resulting pixmap.
     * The image will keep its aspect ratio so this is actually the maximum size for the pixmap, in the best case.
     * @return True if the pixmap has been successfully create, false otherwise.
     */
    bool getCurrentFramePixmap(QPixmap& pixmap, QSize dstSize);

    /**
     * @brief getFirstTwoFrames Gets the first and the second frames stored in this class deque.
     * This is a convenience method to compare both frames in order to detect motion between them.
     * @param [out] first The first frame
     * @param [out] second The second frame
     * @return True if both frames have been set, false if the deque has not the needed two frames.
     */
    bool getFirstTwoFrames (Mat& first, Mat& second);

    /**
     * @brief freeNFrames Takes from the deque the firsts numFramesToFree.
     * @param numFramesToFree The number of frames to remove from the deque.
     */
    void freeNFrames (int numFramesToFree);

    /**
     * @brief freeAllFrames Removes all the frames of the deque.
     * This usually happens when the motion detection system is stopped.
     */
    void freeAllFrames ();

private:
    std::mutex mMutex;
    std::deque<Mat> mFramesBuffer;
    VideoConfig mSrcConfig;
};
