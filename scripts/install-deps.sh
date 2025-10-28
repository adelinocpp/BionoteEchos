#!/bin/bash

# AudioAnnotator Dependency Installation Script
# Instala as dependências necessárias para compilar o projeto

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored messages
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Detect Linux distribution
detect_distro() {
    if [ -f /etc/os-release ]; then
        . /etc/os-release
        echo "$ID"
    else
        echo "unknown"
    fi
}

# Check if running as root
check_root() {
    if [ "$EUID" -eq 0 ]; then
        print_warning "Running as root. This is not recommended."
        read -p "Continue? (y/N) " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            exit 1
        fi
    fi
}

print_info "AudioAnnotator Dependency Installer"
print_info "===================================="
echo ""

# Detect distribution
DISTRO=$(detect_distro)
print_info "Detected distribution: $DISTRO"
echo ""

case "$DISTRO" in
    ubuntu|debian|linuxmint|pop)
        print_info "Installing dependencies for Ubuntu/Debian..."
        
        # Update package list
        print_info "Updating package list..."
        sudo apt-get update
        
        # Install dependencies
        print_info "Installing build tools..."
        sudo apt-get install -y \
            build-essential \
            cmake \
            pkg-config \
            git
        
        print_info "Installing Qt6 packages..."
        sudo apt-get install -y \
            qt6-base-dev \
            qt6-multimedia-dev \
            libqt6multimedia6 \
            libqt6multimediawidgets6 \
            qmake6
        
        print_info "Installing audio libraries..."
        sudo apt-get install -y \
            libfftw3-dev \
            libsndfile1-dev \
            portaudio19-dev \
            libopus-dev \
            libvorbis-dev \
            libflac-dev \
            libmpg123-dev
        
        print_success "All dependencies installed successfully!"
        ;;
        
    fedora|rhel|centos)
        print_info "Installing dependencies for Fedora/RHEL/CentOS..."
        
        print_info "Installing build tools..."
        sudo dnf install -y \
            gcc-c++ \
            cmake \
            pkg-config \
            git
        
        print_info "Installing Qt6 packages..."
        sudo dnf install -y \
            qt6-qtbase-devel \
            qt6-qtmultimedia-devel
        
        print_info "Installing audio libraries..."
        sudo dnf install -y \
            fftw3-devel \
            libsndfile-devel \
            portaudio-devel \
            opus-devel \
            libvorbis-devel \
            flac-devel \
            mpg123-devel
        
        print_success "All dependencies installed successfully!"
        ;;
        
    arch|manjaro)
        print_info "Installing dependencies for Arch Linux..."
        
        sudo pacman -Syu --needed --noconfirm \
            base-devel \
            cmake \
            pkg-config \
            git \
            qt6-base \
            qt6-multimedia \
            fftw \
            libsndfile \
            portaudio \
            opus \
            libvorbis \
            flac \
            mpg123
        
        print_success "All dependencies installed successfully!"
        ;;
        
    opensuse*)
        print_info "Installing dependencies for openSUSE..."
        
        sudo zypper install -y \
            gcc-c++ \
            cmake \
            pkg-config \
            git \
            qt6-base-devel \
            qt6-multimedia-devel \
            fftw3-devel \
            libsndfile-devel \
            portaudio-devel \
            opus-devel \
            libvorbis-devel \
            flac-devel \
            mpg123-devel
        
        print_success "All dependencies installed successfully!"
        ;;
        
    *)
        print_error "Unsupported distribution: $DISTRO"
        print_info "Please install the following packages manually:"
        echo ""
        echo "Build tools:"
        echo "  - gcc/g++ (C++17 compatible)"
        echo "  - cmake (3.16+)"
        echo "  - pkg-config"
        echo ""
        echo "Qt6 packages:"
        echo "  - Qt6Core"
        echo "  - Qt6Widgets"
        echo "  - Qt6Multimedia"
        echo ""
        echo "Audio libraries:"
        echo "  - FFTW3"
        echo "  - libsndfile"
        echo "  - PortAudio (required)"
        echo "  - libopus (optional)"
        echo "  - libvorbis (optional)"
        echo "  - libflac (optional)"
        echo "  - libmpg123 (optional)"
        exit 1
        ;;
esac

echo ""
print_info "Verifying installation..."
echo ""

# Check CMake
if command -v cmake &> /dev/null; then
    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
    print_success "CMake found: $CMAKE_VERSION"
else
    print_error "CMake not found"
fi

# Check pkg-config
if command -v pkg-config &> /dev/null; then
    print_success "pkg-config found"
else
    print_error "pkg-config not found"
fi

# Check FFTW3
if pkg-config --exists fftw3; then
    FFTW_VERSION=$(pkg-config --modversion fftw3)
    print_success "FFTW3 found: $FFTW_VERSION"
else
    print_warning "FFTW3 not found by pkg-config"
fi

# Check libsndfile
if pkg-config --exists sndfile; then
    SNDFILE_VERSION=$(pkg-config --modversion sndfile)
    print_success "libsndfile found: $SNDFILE_VERSION"
else
    print_warning "libsndfile not found by pkg-config"
fi

# Check Qt6
if pkg-config --exists Qt6Core; then
    QT_VERSION=$(pkg-config --modversion Qt6Core)
    print_success "Qt6 found: $QT_VERSION"
else
    print_warning "Qt6 not found by pkg-config"
fi

# Check PortAudio
if pkg-config --exists portaudio-2.0; then
    PORTAUDIO_VERSION=$(pkg-config --modversion portaudio-2.0)
    print_success "PortAudio found: $PORTAUDIO_VERSION"
else
    print_warning "PortAudio not found by pkg-config"
fi

echo ""
print_success "Dependency installation completed!"
print_info "You can now build the project with:"
print_info "  ./scripts/build.sh -d"
