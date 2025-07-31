@echo off
setlocal

echo [INFO] Building JoltPhysics...

:: Set the Jolt path relative to this script
set JOLT_DIR=..\external\JoltPhysics\Build
set BUILD_DIR=%JOLT_DIR%\build

:: Check if CMake exists
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found in PATH.
    pause
    exit /b 1
)

:: Create build directory if it doesn't exist
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

pushd %BUILD_DIR%

:: Generate build files
echo [INFO] Running CMake...
cmake .. -DCMAKE_BUILD_TYPE=Release -DJPH_ENABLE_ASSERTS=ON
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed.
    popd
    pause
    exit /b 1
)

:: Build using MSBuild
echo [INFO] Building with MSBuild...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    popd
    pause
    exit /b 1
)

echo [SUCCESS] Jolt built successfully!
popd
pause