@echo off
echo.
echo  -- Building --
echo.

:: 1. Create the build directory if it doesn't exist
if not exist build mkdir build
cd build

:: 2. Configure the project with CMake
echo [1/2] Running CMake Configuration...
cmake ..
if %ERRORLEVEL% neq 0 (
    echo.
    echo [ERROR] CMake configuration failed!
    pause
    exit /b %ERRORLEVEL%
)

:: 3. Compile the code
echo.
echo [2/2] Compiling the project...
cmake --build .
if %ERRORLEVEL% neq 0 (
    echo.
    echo [ERROR] Build failed!
    pause
    exit /b %ERRORLEVEL%
)

:: 4. Run the executable (if successful)
echo.
echo -- Build Successful! --
echo.

:: Note: Visual Studio (the default Windows CMake generator) puts executables in a "Debug" folder.
if exist Debug\OSMockup.exe (
    echo Starting OS Mockup...
    Debug\OSMockup.exe
) else if exist OSMockup.exe (
    echo Starting OS Mockup...
    OSMockup.exe
) else (
    echo [WARNING] Could not find the executable to run automatically.
)

pause