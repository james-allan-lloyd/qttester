@if "%~1"=="" goto usage

set SOURCE_ROOT=%~dp0
set BUILD_TYPE=%1
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
echo on
cd /D %SOURCE_ROOT%
mkdir build-%BUILD_TYPE%
cd build-%BUILD_TYPE%
conan install .. -s build_type=%BUILD_TYPE% -e CXXFLAGS="-DBOOST_DEBUG_PYTHON=1" --build Boost
if %ERRORLEVEL% neq 0 goto eof
call activate.bat
cmake -G "NMake Makefiles" .. -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if %ERRORLEVEL% neq 0 goto eof
cmake --build . 
goto eof

:usage
@echo Usage: %0 ^<BuildType^>
@exit /B 1

:eof
