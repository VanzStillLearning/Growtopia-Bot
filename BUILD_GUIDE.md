# Growtopia Bot - Modernized Build Guide

This guide covers building the Growtopia Bot with modern tooling and cross-platform support (Windows, Linux, macOS, Termux/Android).

## Prerequisites

### System Requirements
- **CMake**: 3.16 or newer
- **C++ Compiler**: C++17 compatible
  - GCC 7+ (Linux)
  - Clang 5+ (Any platform)
  - MSVC 2019+ (Windows)
- **Qt**: 6.x (preferred) or 5.15+

---

## Building on Termux (Android)

### Quickest Method: Automated Script

```bash
# Clone the repository
git clone https://github.com/VanzStillLearning/Growtopia-Bot.git
cd Growtopia-Bot

# Make script executable
chmod +x build-termux.sh

# Run the build script
./build-termux.sh
```

The script will:
1. ✅ Update Termux packages
2. ✅ Install Qt6, CMake, build tools
3. ✅ Configure with CMake
4. ✅ Build the project
5. ✅ Output binary to `build/growtopia_bot`

### Manual Build (Termux)

```bash
# Step 1: Update packages
pkg update -y

# Step 2: Install dependencies
pkg install -y build-essential cmake qt6-base qt6-tools clang pkg-config

# Step 3: Create build directory
mkdir -p build && cd build

# Step 4: Configure
cmake .. -DCMAKE_BUILD_TYPE=Release

# Step 5: Build
cmake --build . --parallel $(nproc)

# Step 6: Run
./growtopia_bot
```

---

## Building on Linux (Desktop)

### Debian/Ubuntu

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    qt6-base-dev \
    qt6-tools-dev \
    libgl1-mesa-dev \
    pkg-config

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel $(nproc)

# Run
./growtopia_bot
```

### Fedora/RHEL

```bash
# Install dependencies
sudo dnf install -y \
    gcc-c++ \
    cmake \
    qt6-qtbase-devel \
    qt6-qttools-devel \
    mesa-libGL-devel \
    pkg-config

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel $(nproc)

./growtopia_bot
```

### Arch Linux

```bash
sudo pacman -S base-devel cmake qt6-base qt6-tools

mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel $(nproc)

./growtopia_bot
```

---

## Building on macOS

```bash
# Install Homebrew if not present
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake qt6 pkg-config

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=$(brew --prefix qt6)
cmake --build . --parallel $(sysctl -n hw.ncpu)

# Run
./growtopia_bot
```

---

## Building on Windows

### Visual Studio 2022 + Qt6

```bash
# Prerequisites installed:
# 1. Visual Studio 2022 (Community Edition is free)
# 2. Qt 6.x (download from qt.io)
# 3. CMake 3.16+

# Set Qt path (adjust based on your installation)
set QT_PATH=C:\Qt\6.x\msvc2022_64

# Configure
mkdir build
cd build
cmake .. ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_PREFIX_PATH=%QT_PATH% ^
    -G "Visual Studio 17 2022"

# Build
cmake --build . --config Release --parallel %NUMBER_OF_PROCESSORS%

# Run
Release\growtopia_bot.exe
```

### Using Qt Creator (GUI)

1. Open Qt Creator
2. File → Open File or Project → Select `CMakeLists.txt`
3. Configure project with desired kit (MSVC 2022, etc.)
4. Build → Build Project
5. Run with Build → Run

---

## Build Configuration Options

### Release Build (Optimized, no debug symbols)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

### Debug Build (With debug symbols, useful for development)
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
```

### Custom C++ Standard
```bash
# Force C++20
cmake .. -DCMAKE_CXX_STANDARD=20
```

---

## Troubleshooting

### "Qt6 not found"

**Solution**: Install Qt6 development packages or specify path:
```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt6
```

**Fallback to Qt5**:
The CMakeLists.txt automatically falls back to Qt5 if Qt6 is not available.

### "cmake: command not found"

**Linux**: `sudo apt install cmake` (or dnf/pacman equivalent)

**Termux**: `pkg install cmake`

**macOS**: `brew install cmake`

**Windows**: Download from [cmake.org](https://cmake.org/download/)

### Build fails with "ENet not found"

The ENet library is built from source as part of the build. If it fails:

```bash
# Clean and rebuild
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release --verbose
cmake --build . --verbose
```

### Permission denied on build-termux.sh

```bash
chmod +x build-termux.sh
./build-termux.sh
```

### Qt version mismatch

The project now supports both Qt5 and Qt6. CMake will automatically detect and use available version.

Check detected version:
```bash
cmake --version
cmake .. -DCMAKE_BUILD_TYPE=Release
# Look for "Qt Version" in the output
```

---

## Running the Application

After successful build:

### Linux/macOS/Termux
```bash
./build/growtopia_bot
```

### Windows
```bash
.\build\Release\growtopia_bot.exe
```

---

## Project Structure After Build

```
build/
├── growtopia_bot          # Main executable (Linux/macOS/Termux)
├── growtopia_bot.exe      # Main executable (Windows)
├── CMakeFiles/
├── cmake_install.cmake
└── Makefile
```

---

## Advanced: Custom Compiler

### Using Clang instead of GCC

```bash
mkdir build && cd build
cmake .. \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=Release

cmake --build .
```

### Using GCC with optimizations

```bash
cmake .. \
    -DCMAKE_CXX_COMPILER=g++ \
    -DCMAKE_CXX_FLAGS="-O3 -march=native" \
    -DCMAKE_BUILD_TYPE=Release
```

---

## Next Steps

1. **Configure the bot**: Edit the UI or config files
2. **Add credentials**: Use the application to add bot accounts
3. **Run in background**: Consider using `nohup` or `systemd` on Linux

---

## Support

For issues:
1. Check this guide again
2. Review error messages carefully
3. Check your Qt installation path
4. Ensure all dependencies are installed with correct versions

---

## License

GNU Affero General Public License v3.0 - See LICENSE file
