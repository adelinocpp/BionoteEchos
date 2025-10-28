#!/bin/bash

# BionoteEchos Build Script
# Compila a aplicação em modo Debug ou Release

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Default values
BUILD_TYPE="Release"
RUN_AFTER_BUILD=false
CLEAN_BUILD=false
JOBS=$(nproc)

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

# Function to show usage
show_usage() {
    cat << EOF
Usage: $0 [OPTIONS]

Build script for BionoteEchos application.

OPTIONS:
    -d, --debug         Build in Debug mode (default: Release)
    -r, --release       Build in Release mode
    -R, --run           Run the application after building (only in Debug mode)
    -c, --clean         Clean build directory before building
    -j, --jobs N        Number of parallel jobs (default: $(nproc))
    -h, --help          Show this help message

EXAMPLES:
    $0                  # Build in Release mode
    $0 -d               # Build in Debug mode
    $0 -d -R            # Build in Debug mode and run
    $0 -r -c            # Clean build and build in Release mode
    $0 -d -j 4          # Build in Debug mode with 4 parallel jobs

EOF
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            shift
            ;;
        -R|--run)
            RUN_AFTER_BUILD=true
            shift
            ;;
        -c|--clean)
            CLEAN_BUILD=true
            shift
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        -h|--help)
            show_usage
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Set build directory based on build type
BUILD_DIR="$PROJECT_ROOT/build-${BUILD_TYPE,,}"  # Convert to lowercase
BINARY_PATH="$BUILD_DIR/bin/BionoteEchos"

print_info "BionoteEchos Build Script"
print_info "=============================="
print_info "Project Root: $PROJECT_ROOT"
print_info "Build Type: $BUILD_TYPE"
print_info "Build Directory: $BUILD_DIR"
print_info "Parallel Jobs: $JOBS"
echo ""

# Clean build if requested
if [ "$CLEAN_BUILD" = true ]; then
    if [ -d "$BUILD_DIR" ]; then
        print_warning "Cleaning build directory: $BUILD_DIR"
        rm -rf "$BUILD_DIR"
        print_success "Build directory cleaned"
    else
        print_info "Build directory does not exist, nothing to clean"
    fi
fi

# Create build directory
if [ ! -d "$BUILD_DIR" ]; then
    print_info "Creating build directory: $BUILD_DIR"
    mkdir -p "$BUILD_DIR"
fi

# Navigate to build directory
cd "$BUILD_DIR"

# Run CMake
print_info "Running CMake..."
if cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE "$PROJECT_ROOT"; then
    print_success "CMake configuration completed"
else
    print_error "CMake configuration failed"
    exit 1
fi

echo ""

# Build the project
print_info "Building BionoteEchos ($BUILD_TYPE mode) with $JOBS parallel jobs..."
if make -j$JOBS; then
    print_success "Build completed successfully!"
else
    print_error "Build failed"
    exit 1
fi

echo ""

# Check if binary exists
if [ -f "$BINARY_PATH" ]; then
    print_success "Binary created at: $BINARY_PATH"
    
    # Show binary size
    BINARY_SIZE=$(du -h "$BINARY_PATH" | cut -f1)
    print_info "Binary size: $BINARY_SIZE"
else
    print_error "Binary not found at expected location: $BINARY_PATH"
    exit 1
fi

echo ""

# Run the application if requested (only in Debug mode)
if [ "$RUN_AFTER_BUILD" = true ]; then
    if [ "$BUILD_TYPE" = "Debug" ]; then
        print_info "Running BionoteEchos in Debug mode..."
        echo ""
        print_info "=============================="
        print_info "Application Output:"
        print_info "=============================="
        "$BINARY_PATH"
    else
        print_warning "Run option (-R/--run) only works in Debug mode"
        print_info "To run the Release build, execute: $BINARY_PATH"
    fi
else
    print_info "To run the application, execute:"
    print_info "  $BINARY_PATH"
fi

echo ""
print_success "Build process completed!"
