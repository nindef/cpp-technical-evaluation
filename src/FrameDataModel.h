#pragma once

#include "FrameWrapper.h"

#include <QPixmap>
#include <deque>
#include <mutex>

/**
 * @brief The IFrameDataModel class manages the acquired frames.
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
    FrameDataModel::VideoConfig& getSrcVideoConfig()
    {
        return mSrcConfig;
    }

    /**
     * @brief storeFrame Stores an acquired frame (from FramAcquisitor) into this class deque
     * @param frame The frame to store
     */
    void storeFrame(std::shared_ptr<IFrameType> frame)
    {
        mMutex.lock();
        mFramesBuffer.push_back(frame);
        mMutex.unlock();
    }

    /**
     * @brief getFirstFrame Gets the first frame buffered
     * @return The first frame
     */
    std::shared_ptr<IFrameType> getFirstFrame ()
    {
        std::shared_ptr<IFrameType> frame;
        mMutex.lock();
        if (!mFramesBuffer.empty())
            frame = mFramesBuffer[0];
        mMutex.unlock();
        return frame;
    }

    /**
     * @brief getFirstTwoFrames Gets the first and the second frames stored in this class deque.
     * This is a convenience method to compare both frames in order to detect motion between them.
     * @return A pair with the first and the second frames. If there are no two frames available
     * in the buffered deque, then both frames are nullptr.
     */
    std::pair<std::shared_ptr<IFrameType>, std::shared_ptr<IFrameType>> getFirstTwoFrames ()
    {
        std::pair<std::shared_ptr<IFrameType>, std::shared_ptr<IFrameType>> pair (nullptr, nullptr);

        mMutex.lock();
        if (mFramesBuffer.size() >= 2)
        {
            pair.first = mFramesBuffer[0];
            pair.second = mFramesBuffer[1];
        }
        mMutex.unlock();

        return pair;
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
    std::mutex mMutex;
    std::deque<std::shared_ptr<IFrameType>> mFramesBuffer;
    VideoConfig mSrcConfig;

    virtual const unsigned char* getData(IFrameType& frame)
    {
        return frame.getData();
    };

    virtual uint getCols(IFrameType& frame)
    {
        return frame.getCols();
    };

    virtual uint getRows(IFrameType& frame)
    {
        return frame.getRows();
    };
};
