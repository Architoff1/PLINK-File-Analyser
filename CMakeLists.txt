cmake_minimum_required(VERSION 3.16)

project(PlinkPCAAnalyzer)

set(CMAKE_CXX_STANDARD 20)

# Enable Qt automatic processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(plink_pca_tool
    src/main.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.h
    src/plink/PlinkLoader.cpp
)

target_include_directories(plink_pca_tool PRIVATE src)

target_link_libraries(plink_pca_tool Qt6::Widgets)