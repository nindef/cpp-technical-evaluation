
add_executable(MotionRecorder main.cpp
                          MainWindow.cpp
                          MainWindow.h
                          MotionRecorderWidget.h
                          MotionRecorderWidget.cpp
                          FrameWrapper.h
                          AFrameAcquisitor.h
                          CV_FrameAcquisitor.h
                          CV_FrameAcquisitor.cpp
                          MotionController.h
                          MotionController.cpp
                          FrameDataModel.h
                          AFrameWriter.h
                          CV_FrameWriter.h
                          CV_FrameWriter.cpp
                          AMotionRecorderManager.h
                          AMotionRecorderManager.cpp
                          CV_MotionRecorderManager.h
                          CV_MotionRecorderManager.cpp
                          motion_detector/include/motion_detector.hpp
                          resources.qrc
                          ${APP_ICON_RESOURCE_WINDOWS})


find_package(OpenCV REQUIRED COMPONENTS core highgui)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

target_link_libraries(MotionRecorder ${OpenCV_LIBS} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Multimedia)

deployTarget(MotionRecorder ${OUTPUT_BIN_DIR}/../${CMAKE_BUILD_TYPE}_Deploy)
