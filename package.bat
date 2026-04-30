@echo off
echo Packaging Chess Game...
echo.

REM Create deployment folder
if exist ChessGame_Deploy rmdir /S /Q ChessGame_Deploy
mkdir ChessGame_Deploy

REM Copy executable
echo Copying executable...
copy ChessGUI.exe ChessGame_Deploy\
if errorlevel 1 echo ERROR: Failed to copy ChessGUI.exe

REM Copy SFML DLLs
echo Copying SFML DLLs...
copy C:\SFML-2.6.1\bin\sfml-graphics-2.dll ChessGame_Deploy\
if errorlevel 1 echo ERROR: Failed to copy sfml-graphics-2.dll

copy C:\SFML-2.6.1\bin\sfml-window-2.dll ChessGame_Deploy\
if errorlevel 1 echo ERROR: Failed to copy sfml-window-2.dll

copy C:\SFML-2.6.1\bin\sfml-system-2.dll ChessGame_Deploy\
if errorlevel 1 echo ERROR: Failed to copy sfml-system-2.dll

REM Copy MinGW DLLs
echo Copying MinGW DLLs...
copy C:\mingw\mingw64\bin\libgcc_s_seh-1.dll ChessGame_Deploy\
if errorlevel 1 echo ERROR: Failed to copy libgcc_s_seh-1.dll

Copy C:\mingw\mingw64\bin\libstdc++-6.dll ChessGame_Deploy\
if errorlevel 1 echo ERROR: Failed to copy libstdc++-6.dll

copy C:\mingw\mingw64\bin\libwinpthread-1.dll ChessGame_Deploy\
if errorlevel 1 echo ERROR: Failed to copy libwinpthread-1.dll

REM Copy assets
echo Copying assets...
copy Roboto-Regular.ttf ChessGame_Deploy\
if errorlevel 1 echo ERROR: Failed to copy Roboto-Regular.ttf

xcopy pieces ChessGame_Deploy\pieces\ /E /I /Y
if errorlevel 1 echo ERROR: Failed to copy pieces folder

echo.
echo ========================================
echo Files in ChessGame_Deploy:
echo ========================================
dir /B ChessGame_Deploy
echo.
echo ========================================
echo DLLs in ChessGame_Deploy:
echo ========================================
dir /B ChessGame_Deploy\*.dll
echo.
echo Done! Package is in ChessGame_Deploy folder
pause