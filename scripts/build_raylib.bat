@echo off
echo Building raylib.lib with /MTd...

REM Clone raylib if not already cloned
if not exist ..\external\raylib (
    git clone https://github.com/raysan5/raylib.git
)

cd ..\external\raylib

REM Configure for MTd
cmake -B build -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDebug" -DCMAKE_BUILD_TYPE=Debug

REM Build
cmake --build build --config Debug

echo Done! Library is at: raylib\build\Debug\raylib.lib