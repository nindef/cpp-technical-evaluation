
add_executable(MotionRecorder main.cpp
                          MainWindow.cpp
                          MainWindow.h
                          FrameAcquisitor.h
                          FrameAcquisitor.cpp
                          MotionController.h
                          MotionController.cpp
                          FrameDataModel.h
                          FrameDataModel.cpp
                          FrameWriter.h
                          FrameWriter.cpp
                          CameraMotionDetection.h
                          CameraMotionDetection.cpp
                          CameraMotionDetectionWidget.h
                          CameraMotionDetectionWidget.cpp
                          motion_detector/include/motion_detector.hpp
                          resources.qrc
                      )

if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${MotionRecorder_SOURCE_DIR}/bin/release)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${MotionRecorder_SOURCE_DIR}/bin/debug)
endif()

find_package(OpenCV REQUIRED COMPONENTS core highgui)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets MultimediaWidgets Multimedia)

target_link_libraries(MotionRecorder ${OpenCV_LIBS} Qt5::Core Qt5::Gui Qt5::Widgets Qt5::MultimediaWidgets Qt5::Multimedia)
