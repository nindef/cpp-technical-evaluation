#pragma once

#include "AMotionRecorderManager.h"

#include <iostream>
#include <thread>

#include <QPixmap>

namespace cv { class Mat; }
class FrameDataModel;
class MotionController;
class CV_FrameAcquisitor;

class CV_MotionRecorderManager : public AMotionRecorderManager
{
public:
    CV_MotionRecorderManager (std::string sourceStreamPath, std::string outputVideoBaseName);

    void buildMembers () override;
};
