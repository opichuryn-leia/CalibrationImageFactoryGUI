# CalibrationImageFactoryGUI
Lightweight UI for calibrating Leia displays.

Requirements:
- Qt 5.15
- C++14 compatible compiler

How to build:
- cmake.exe path/to/CalibrationImageFactoryGUI && cmake --build .

How to deploy (make delivery package):
- update CalibrationImageFactoryGUI/deploy/win/windows_env.bat according your environment
- run windows_env.bat
- do build
- make (ninja, jom, ...) deploy
