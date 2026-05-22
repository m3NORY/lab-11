
cmake_minimum_required(VERSION 3.16)

project(PointCloudEditor)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(PointCloudEditor
    main.cpp

    core/matrix4x4.cpp
    core/pointcloud.cpp
    core/transform.cpp

    gui/mainwindow.cpp

    io/jsonserializer.cpp

    core/matrix4x4.h
    core/pointcloud.h
    core/transform.h

    gui/mainwindow.h

    io/jsonserializer.h
)

target_link_libraries(PointCloudEditor PRIVATE Qt6::Widgets)
