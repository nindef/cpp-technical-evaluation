#pragma once

#include <QPixmap>
#include <deque>
#include <mutex>

/**
 * @brief The IFrameDataModel class manages the acquired frames.
 */
template<typename FD>
class IFrameDataModel
{
public:
    /**
     * @brief The SourceVideoConfig struct stores the basic information from a video.
     */
    struct VideoConfig{
        double fourcc = 0;
        double fps = 0;
        int width = 0;
        int height = 0;

        VideoConfig (){}
        VideoConfig (double fourcc, double fps, int width, int height)
        {
            this->fourcc = fourcc;
            this->fps = fps;
            this->width = width;
            this->height = height;
        }
    };

    /**
     * @brief setSrcVideoConfig Sets the source video config to store it.
     * The VideoWriter will use this same configuration to set its parameters.
     * @param fourcc The FourCC code.
     * @param fps The frames per second.
     * @param width The frame width.
     * @param height The frame height.
     */
    void setSrcVideoConfig (VideoConfig& videoConfig)
    {
        mSrcConfig = videoConfig;
    }

    /**
     * @brief getSrcVideoConfig Gets the source video config, stored before.
     * @return The videoConfig for the source video.
     */
    IFrameDataModel::VideoConfig& getSrcVideoConfig()
    {
        return mSrcConfig;
    }

    /**
     * @brief storeFrame Stores an acquired frame (from FramAcquisitor) into this class deque
     * @param frame The frame to store
     */
    void storeFrame(FD frame)
    {
        mMutex.lock();
        mFramesBuffer.push_back(std::move(frame));
        mMutex.unlock();
    }

    /**
     * @brief getCurrentFramePixmap Gets the current frame (frame in first place in deque) as a QPixmap
     * @param pixmap The destiny QPixmap
     * @param dstSize The desired size for the resulting pixmap.
     * The image will keep its aspect ratio so this is actually the maximum size for the pixmap, in the best case.
     * @return True if the pixmap has been successfully create, false otherwise.
     */
    bool getCurrentFramePixmap(QPixmap& pixmap, QSize dstSize)
    {
        FD frame;
        mMutex.lock();
        if (!mFramesBuffer.empty())
        {
            frame = mFramesBuffer[0];
            QImage image(getData(frame), getCols(frame), getRows(frame), QImage::Format_RGB888);
            image = image.rgbSwapped ();
            image = image.scaled(dstSize, Qt::KeepAspectRatio);
            pixmap = QPixmap::fromImage(image);

            mMutex.unlock();
            return true;
        }

        mMutex.unlock();
        return false;
    }

    /**
     * @brief getFirstTwoFrames Gets the first and the second frames stored in this class deque.
     * This is a convenience method to compare both frames in order to detect motion between them.
     * @param [out] first The first frame
     * @param [out] second The second frame
     * @return True if both frames have been set, false if the deque has not the needed two frames.
     */
    bool getFirstTwoFrames (FD& first, FD& second)
    {
        bool areFramesSet = false;
        mMutex.lock();
        if (mFramesBuffer.size() >= 2)
        {
            first = mFramesBuffer[0];
            second = mFramesBuffer[1];
            areFramesSet = true;
        }
        mMutex.unlock();
        return areFramesSet;
    }

    /**
     * @brief freeNFrames Takes from the deque the firsts numFramesToFree.
     * @param numFramesToFree The number of frames to remove from the deque.
     */
    void freeNFrames (int numFramesToFree)
    {
        mMutex.lock();
        for (auto index = 0; index < numFramesToFree; index++)
        {
            if (!mFramesBuffer.empty())
                mFramesBuffer.pop_front();
        }
        mMutex.unlock();
    }

    /**
     * @brief freeAllFrames Removes all the frames of the deque.
     * This usually happens when the motion detection system is stopped.
     */
    void freeAllFrames ()
    {
        mMutex.lock();
        mFramesBuffer.clear();
        mMutex.unlock();
    }

protected:
    virtual unsigned char* getData(FD& frame) = 0;
    virtual uint getCols(FD& frame) = 0;
    virtual uint getRows(FD& frame) = 0;

private:
    std::mutex mMutex;
    std::deque<FD> mFramesBuffer;
    VideoConfig mSrcConfig;
};
