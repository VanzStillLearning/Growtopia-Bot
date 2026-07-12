# Growtopia Bot - Modernized Edition

> A cross-platform Growtopia bot built with modern C++17, Qt6, and CMake. Now with native Termux/Android support!

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue)](https://en.cppreference.com/w/cpp/17)
[![Qt6](https://img.shields.io/badge/Qt-6-green)](https://www.qt.io/)
[![CMake 3.16+](https://img.shields.io/badge/CMake-3.16%2B-red)](https://cmake.org/)
[![License: AGPL-3.0](https://img.shields.io/badge/License-AGPL%203.0-yellow)](./LICENSE)

**Platforms**: Windows • Linux • macOS • Termux/Android

---

## ✨ Features

- 🔐 Secure login handling with updated authentication
- 🎮 Growtopia game automation
- 🌐 ENet-based networking
- 💻 Cross-platform support
- 📱 Native Android support via Termux
- ⚡ Modern C++17 with smart memory management
- 🔄 Qt6 GUI framework
- 🏗️ CMake build system

---

## 🚀 Quick Start

### Termux (Android)
```bash
git clone https://github.com/VanzStillLearning/Growtopia-Bot.git
cd Growtopia-Bot
chmod +x build-termux.sh
./build-termux.sh
```

### Linux
```bash
git clone https://github.com/VanzStillLearning/Growtopia-Bot.git
cd Growtopia-Bot
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel $(nproc)
./growtopia_bot
```

### macOS
```bash
brew install cmake qt6
git clone https://github.com/VanzStillLearning/Growtopia-Bot.git
cd Growtopia-Bot
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
./growtopia_bot
```

### Windows
```cmd
git clone https://github.com/VanzStillLearning/Growtopia-Bot.git
cd Growtopia-Bot
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH="C:\Qt\6.x\msvc2022_64" -G "Visual Studio 17 2022"
cmake --build . --config Release
Release\growtopia_bot.exe
```

---

## 📋 Requirements

### Termux (Android)
```bash
pkg install -y build-essential cmake qt6-base clang
```

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install -y build-essential cmake qt6-base-dev qt6-tools-dev pkg-config
```

### macOS
```bash
brew install cmake qt6 pkg-config
```

### Windows
- Visual Studio 2019+ or Clang
- CMake 3.16+
- Qt 6.x SDK

---

## 📚 Documentation

- **[BUILD_GUIDE.md](./BUILD_GUIDE.md)** - Detailed build instructions for all platforms
- **[MODERNIZATION.md](./MODERNIZATION.md)** - Technical improvements from 2013 to 2024
- **[Original README](./README.md)** - Original project information

---

## 🏗️ Project Structure

```
Growtopia-Bot/
├── CMakeLists.txt              # Modern build configuration
├── build-termux.sh             # Automated Termux builder
├── BUILD_GUIDE.md              # Comprehensive guide
├── MODERNIZATION.md            # Upgrade documentation
├── README.md                   # Original README
├── Growtopia_Bot/
│   ├── main.cpp               # Entry point
│   ├── mainwindow.cpp/h       # GUI
│   ├── corefunc.h             # Core bot logic
│   ├── corefunc_modern.h      # Modern C++17 version
│   ├── userfunc.h             # User functions
│   ├── utilsfunc.h            # Utilities
│   ├── enet2/                 # ENet networking library
│   ├── include/               # Headers
│   └── Growtopia_Bot.pro      # Original qmake (legacy)
└── build/                      # Build output (after build)
```

---

## 🔧 Build System

This project uses **CMake 3.16+** for cross-platform builds.

### Standard Build
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel $(nproc)
```

### With Specific Compiler
```bash
cmake .. -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Release
```

### Debug Build
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

---

## 🤖 Running the Bot

After build:

```bash
# Linux/macOS/Termux
./build/growtopia_bot

# Windows
.\build\Release\growtopia_bot.exe
```

---

## 🔄 Upgrading from Old Version

If you have the original (2013) version:

1. **Backup** your configuration
2. **Clone** this modernized version
3. **Build** using the new CMake system
4. **Import** your old configs (if compatible)

---

## 📝 Code Examples

### Modern C++17 Memory Management
```cpp
// Old way (C++98)
BYTE* data = new BYTE[56];
delete[] data;  // Easy to forget!

// New way (C++17)
auto data = std::make_unique<std::array<uint8_t, 56>>();
// Automatically cleaned up
```

### Constructor Improvements
```cpp
// Old way
GrowtopiaBot::GrowtopiaBot(string username, string password)
{
    this->uname = username;
    this->upass = password;
}

// New way (C++17)
GrowtopiaBot(const string& username, const string& password)
    : uname(username), upass(password) {}
```

---

## 🐛 Troubleshooting

### CMake not found
- **Linux**: `sudo apt-get install cmake`
- **macOS**: `brew install cmake`
- **Termux**: `pkg install cmake`
- **Windows**: Download from [cmake.org](https://cmake.org/)

### Qt not found
```bash
# Specify Qt path
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt6
```

### Build fails
```bash
# Clean and rebuild
rm -rf build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release --verbose
cmake --build . --verbose
```

See **[BUILD_GUIDE.md](./BUILD_GUIDE.md)** for more troubleshooting.

---

## 🔐 License

GNU Affero General Public License v3.0

You must:
- ✅ Disclose source code modifications
- ✅ Include original license
- ✅ Document all changes

See [LICENSE](./LICENSE) for details.

---

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Build and test locally
5. Submit a pull request

---

## 📞 Support

- 📖 Check [BUILD_GUIDE.md](./BUILD_GUIDE.md)
- 🔍 Review [MODERNIZATION.md](./MODERNIZATION.md)
- 🐛 Create an issue on GitHub

---

## 🎯 Roadmap

- [x] CMake modernization
- [x] Termux/Android support
- [x] Qt6 compatibility
- [x] C++17 upgrade
- [ ] Qt6 default build
- [ ] Async networking (C++20 coroutines)
- [ ] Better error handling
- [ ] Unit tests

---

## 📦 Version Info

- **Current**: 2.0 (Modernized)
- **Original**: 1.0 (2013)
- **Build System**: CMake 3.16+
- **C++ Standard**: C++17
- **Qt Version**: 6.x (fallback to 5.15+)

---

Made with ❤️ for the Growtopia community

**Original Authors**: Growtopia Noobs  
**Modernization**: 2024
