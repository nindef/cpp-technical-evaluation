
find_package(Qt5Core REQUIRED)

get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

#get_target_property(__dll_release opencv_world  IMPORTED_LOCATION_RELEASE)
#add_library(opencv_world SHARED IMPORTED)
#get_target_property(_opencv_world_dll opencv_world IMPORTED_LOCATION_RELEASE)

function(deployTarget target DEPLOY_DIR)
    add_custom_command(TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "${DEPLOY_DIR}"
        COMMAND set PATH="${_qt_bin_dir}"
        COMMAND "${WINDEPLOYQT_EXECUTABLE}" --dir "${DEPLOY_DIR}" --no-compiler-runtime --no-translations --no-angle --release --no-opengl-sw "$<TARGET_FILE:${target}>"
        COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${target}>" "${DEPLOY_DIR}/$<TARGET_FILE_NAME:${target}>"
        COMMAND ${CMAKE_COMMAND} -E copy "${_OpenCV_LIB_PATH}/opencv_world3416.dll" ${DEPLOY_DIR}
)
endfunction()

