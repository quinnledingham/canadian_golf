@echo off

set SDL_SDK=C:\Users\quinn\Libs\SDL3-3.2.26
set SDL_TTF_SDK=C:\Users\quinn\Libs\SDL3_ttf-3.1.0

set CF_SDL=/I%SDL_SDK%\include\SDL3 /I%SDL_SDK%\include -DSDL /I%SDL_TTF_SDK%\include
set CF=-nologo -Z7 -W3 -MD %CF_SDL% /Fe:golf.exe -D_CRT_SECURE_NO_WARNINGS /std:c++20 -DDEBUG

set LF_SDL=%SDL_SDK%\lib\x64\SDL3.lib %SDL_TTF_SDK%\lib\x64\SDL3_ttf.lib
set LF=-incremental:no -subsystem:console %LF_SDL%


cls

:: create/move into build directory
pushd %CD%
IF NOT EXIST build mkdir build
cd build

:: copy dlls if needed
IF NOT EXIST SDL3.dll copy %SDL_SDK%\lib\x64\SDL3.dll
IF NOT EXIST SDL3_ttf.dll copy %SDL_TTF_SDK%\lib\x64\SDL3_ttf.dll

:: COMPILE
cl %CF% ../golf_sdl.cpp /link %LF%

:: move out of build directory
cd ..