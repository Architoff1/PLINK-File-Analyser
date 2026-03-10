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
    src/plink/PlinkLoader.h
    src/plink/BedReader.cpp
    src/plink/BedReader.h
    src/genome/GenomeDetector.cpp
    src/genome/GenomeDetector.h
    src/data/GenotypeBlock.cpp
    src/data/GenotypeBlock.h
    src/qc/QCEngine.cpp
    src/qc/QCEngine.h
    src/qc/QCWorker.cpp
    src/qc/QCWorker.h
    src/qc/QCParallelEngine.cpp
    src/qc/QCParallelEngine.h
)

target_include_directories(plink_pca_tool PRIVATE src)

target_link_libraries(plink_pca_tool Qt6::Widgets)