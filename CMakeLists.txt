cmake_minimum_required(VERSION 3.16)

project(PlinkPCAAnalyzer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(plink_pca_tool
    src/main.cpp
)

target_link_libraries(plink_pca_tool
    Qt6::Widgets
)