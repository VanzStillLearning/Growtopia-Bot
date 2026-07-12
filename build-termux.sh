#!/bin/bash

# Growtopia Bot - Termux Build Script
# This script sets up and builds the project for Termux/Android

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}Growtopia Bot - Termux Build Script${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""

# Check if running in Termux
if [ ! -d "$PREFIX" ]; then
    echo -e "${RED}Error: This script must be run in Termux!${NC}"
    echo "Get Termux from: https://termux.com"
    exit 1
fi

echo -e "${YELLOW}[1/5] Updating package manager...${NC}"
pkg update -y

echo -e "${YELLOW}[2/5] Installing dependencies...${NC}"
pkg install -y \
    build-essential \
    cmake \
    qt6-base \
    qt6-tools \
    clang \
    git \
    pkg-config

echo -e "${YELLOW}[3/5] Creating build directory...${NC}"
mkdir -p build
cd build

echo -e "${YELLOW}[4/5] Configuring CMake...${NC}"
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++

echo -e "${YELLOW}[5/5] Building project...${NC}"
cmake --build . --parallel $(nproc)

echo ""
echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}Build completed successfully!${NC}"
echo -e "${GREEN}========================================${NC}"
echo ""
echo -e "Binary location: ${GREEN}$(pwd)/growtopia_bot${NC}"
echo -e "To run: ${GREEN}./growtopia_bot${NC}"
echo ""
