echo off

echo Preparing ACF framework

copy /Y %QTDIR%/bin/*.dll Bin/Release%COMPILER_EXT%
copy /Y %QTDIR%/bin/*.dll Bin/Debug%COMPILER_EXT%

echo Preparing ACF framework done

