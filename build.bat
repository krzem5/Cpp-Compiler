@echo off
cls
set INCLUDE=%INCLUDE%;.\..\;
rmdir /s /q .\build\
mkdir build
cd build
cl /FC /EHsc /std:c++17 ..\*.cpp /link /OUT:index.exe&&del *.obj&&xcopy .\..\lib .\lib\ /Y /H /R&&cls&&index.exe .\..\test.FILE
if exist *.obj del *.obj
cd ../
