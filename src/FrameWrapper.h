#pragma once

#include <QPixmap>
#include <mutex>
#include <iostream>

#include <opencv2/core/mat.hpp>
using namespace cv;

    class IFrameType
    {
    public:
        virtual ~IFrameType () = default;
        virtual QPixmap getCurrentFramePixmap(QSize dstSize) = 0;
        virtual const unsigned char *getData() = 0;
		virtual uint getCols() = 0;
        virtual uint getRows() = 0;
    };

    template<typename T>
    class AFrameType : public IFrameType
    {
    public:
        std::shared_ptr<T> getFrame () {return mFrame;}

    protected:
        std::shared_ptr<T> mFrame;
    };

    // The derived type for OpenCV
    class CVFrameWrapper : public AFrameType<Mat>
    {
    public:
        CVFrameWrapper(std::shared_ptr<Mat>(frame))
        {
            mFrame = frame;
        }

        QPixmap getCurrentFramePixmap(QSize dstSize) override
        {
            QImage image(getData(), getCols(), getRows(), QImage::Format_RGB888);
            image = image.rgbSwapped ();
            image = image.scaled(dstSize, Qt::KeepAspectRatio);
            const auto pixmap = QPixmap::fromImage(image);
            return pixmap;
        }

        const unsigned char *getData() override
        {
            return static_cast<unsigned char*>(mFrame->data);
        }

        uint getCols() override
        {
            return mFrame->cols;
        }

        uint getRows() override
        {
            return mFrame->rows;
        }
    };


    // The derived type for Qt
    class QtFrameWrapper : public AFrameType<QImage>
    {
    public:
        QtFrameWrapper(std::shared_ptr<QImage>(frame))
        {
            mFrame = frame;
        }

        QPixmap getCurrentFramePixmap(QSize dstSize) override
        {
            QImage image(getData(), getCols(), getRows(), QImage::Format_RGB888);
            image = image.rgbSwapped ();
            image = image.scaled(dstSize, Qt::KeepAspectRatio);
            const auto pixmap = QPixmap::fromImage(image);
            return pixmap;
        }

        const unsigned char *getData() override
        {
            return mFrame->bits();
        }

        uint getCols() override
        {
            return mFrame->width();
        }

        uint getRows() override
        {
            return mFrame->height();
        }
    };
