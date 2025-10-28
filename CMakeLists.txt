cmake_minimum_required(VERSION 3.16)

project(BionoteEchos VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Habilitar suporte a traduções Qt
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia Concurrent)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTW3 REQUIRED fftw3)
pkg_check_modules(SNDFILE REQUIRED sndfile)
pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)

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
    ${PORTAUDIO_INCLUDE_DIRS}
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
    src/views/AboutDialog.cpp
    src/views/TimeEdit.cpp
    src/views/AudioMetadataDialog.cpp
    src/views/SpectrogramSettingsDialog.cpp
    src/models/AudioFile.cpp
    src/models/AnnotationTier.cpp
    src/models/AnnotationInterval.cpp
    src/models/AnnotationPoint.cpp
    src/models/Project.cpp
    src/audio/AudioDecoder.cpp
    src/audio/AudioDecoderWorker.cpp
    src/audio/AudioPlayer.cpp
    src/audio/CustomAudioPlayer.cpp
    src/audio/SpectrogramCalculator.cpp
    src/audio/PitchDetector.cpp
    src/audio/IntensityCalculator.cpp
    src/controllers/ProjectController.cpp
    src/controllers/AudioController.cpp
    src/controllers/AnnotationController.cpp
    src/utils/TextGridExporter.cpp
    src/utils/TextGridImporter.cpp
    src/utils/Logger.cpp
)

# Resource files
set(RESOURCES
    resources/resources.qrc
)

# Header files
set(HEADERS
    include/views/MainWindow.h
    include/views/AudioListWidget.h
    include/views/AudioVisualizationWidget.h
    include/views/AnnotationLayerWidget.h
    include/views/SpectrogramWidget.h
    include/views/AudioControlWidget.h
    include/views/AboutDialog.h
    include/views/TimeEdit.h
    include/views/AudioMetadataDialog.h
    include/views/SpectrogramSettingsDialog.h
    include/models/AudioFile.h
    include/models/AnnotationTier.h
    include/models/AnnotationInterval.h
    include/models/AnnotationPoint.h
    include/models/Project.h
    include/audio/AudioDecoder.h
    include/audio/AudioDecoderWorker.h
    include/audio/AudioPlayer.h
    include/audio/CustomAudioPlayer.h
    include/audio/SpectrogramCalculator.h
    include/audio/PitchDetector.h
    include/audio/IntensityCalculator.h
    include/controllers/ProjectController.h
    include/controllers/AudioController.h
    include/controllers/AnnotationController.h
    include/utils/TextGridExporter.h
    include/utils/TextGridImporter.h
    include/utils/Logger.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${RESOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::Concurrent
    ${FFTW3_LIBRARIES}
    ${SNDFILE_LIBRARIES}
    ${PORTAUDIO_LIBRARIES}
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

