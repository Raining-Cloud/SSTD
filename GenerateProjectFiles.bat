@echo off
if not exist "Build" mkdir Build
cd Build

cmake ../
echo Done!
pause