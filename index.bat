@echo off
cls
set LIB=C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.25.28610\lib\x64;C:\Program Files (x86)\Windows Kits\NETFXSDK\4.8\lib\um\x64;C:\Program Files (x86)\Windows Kits\10\lib\10.0.18362.0\ucrt\x64;C:\Program Files (x86)\Windows Kits\10\lib\10.0.18362.0\um\x64;
set INCLUDE=C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Tools\MSVC\14.25.28610\include;C:\Program Files (x86)\Windows Kits\NETFXSDK\4.8\include\um;C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\ucrt;C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\shared;C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\um;C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\winrt;C:\Program Files (x86)\Windows Kits\10\include\10.0.18362.0\cppwinrt
del *.obj&&del index.exe&&cl /EHsc *.cpp /link /OUT:index.exe&&del *.obj&&cls&&index.exe test.FILE
if exist index.exe (
	rem del index.exe
)
