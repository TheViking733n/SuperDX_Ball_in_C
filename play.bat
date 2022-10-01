@echo off
cls
echo Compiling the code...
echo.
echo Please wait...
title Super DX Ball by The Viking
gcc game.c -o "%temp%\game.exe"
echo.
echo.
echo Done!
"%temp%\game.exe"
cls
echo.
echo.
echo.
echo                    Made by The Viking
timeout /t 4 /nobreak >nul
exit
REM End of file
