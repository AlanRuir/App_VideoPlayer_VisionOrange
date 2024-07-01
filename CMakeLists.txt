cmake_minimum_required(VERSION 3.5)

project(VideoPlayer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Qml Quick Multimedia REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(CUDA REQUIRED)

add_executable(VideoPlayer 
    src/main.cpp 
    src/video_player_controller.cpp 
    src/video_frame_provider.cpp
    src/main.qrc)

target_include_directories(VideoPlayer PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(VideoPlayer PRIVATE Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Multimedia avcodec avformat avutil swscale)