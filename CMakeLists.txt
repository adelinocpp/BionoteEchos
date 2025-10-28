cmake_minimum_required(VERSION 3.16)

project(AudioAnnotator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Habilitar suporte a traduções Qt
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)

# Find FFTW3
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTW3 REQUIRED fftw3)

# Find libsndfile
pkg_check_modules(SNDFILE REQUIRED sndfile)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/models
    ${CMAKE_SOURCE_DIR}/include/views
    ${CMAKE_SOURCE_DIR}/include/controllers
    ${CMAKE_SOURCE_DIR}/include/audio
    ${CMAKE_SOURCE_DIR}/include/utils
    ${FFTW3_INCLUDE_DIRS}
    ${SNDFILE_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    src/main.cpp
    src/views/MainWindow.cpp
    src/views/AudioListWidget.cpp
    src/views/AudioVisualizationWidget.cpp
    src/views/AnnotationLayerWidget.cpp
    src/views/SpectrogramWidget.cpp
    src/views/AudioControlWidget.cpp
    src/models/AudioFile.cpp
    src/models/AnnotationTier.cpp
    src/models/AnnotationInterval.cpp
    src/models/AnnotationPoint.cpp
    src/models/Project.cpp
    src/audio/AudioDecoder.cpp
    src/audio/AudioPlayer.cpp
    src/audio/SpectrogramCalculator.cpp
    src/audio/PitchDetector.cpp
    src/audio/IntensityCalculator.cpp
    src/controllers/ProjectController.cpp
    src/controllers/AudioController.cpp
    src/controllers/AnnotationController.cpp
    src/utils/TextGridExporter.cpp
    src/utils/TextGridImporter.cpp
)

# Header files
set(HEADERS
    include/views/MainWindow.h
    include/views/AudioListWidget.h
    include/views/AudioVisualizationWidget.h
    include/views/AnnotationLayerWidget.h
    include/views/SpectrogramWidget.h
    include/views/AudioControlWidget.h
    include/models/AudioFile.h
    include/models/AnnotationTier.h
    include/models/AnnotationInterval.h
    include/models/AnnotationPoint.h
    include/models/Project.h
    include/audio/AudioDecoder.h
    include/audio/AudioPlayer.h
    include/audio/SpectrogramCalculator.h
    include/audio/PitchDetector.h
    include/audio/IntensityCalculator.h
    include/controllers/ProjectController.h
    include/controllers/AudioController.h
    include/controllers/AnnotationController.h
    include/utils/TextGridExporter.h
    include/utils/TextGridImporter.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    ${FFTW3_LIBRARIES}
    ${SNDFILE_LIBRARIES}
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

