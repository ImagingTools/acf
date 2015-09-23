rem echo off

set COMPILER_EXT=VC10
set QMAKESPEC=%QTDIR%\mkspecs\win32-msvc2010
set path=%path%;%QTDIR%\bin

echo Generating %COMPILER_EXT% projects...

cd %~dp0\..\..\Build\QMake
%QTDIR%\bin\qmake -recursive -tp vc

cd %~dp0\..\..\Docs\Tutorial\Build\QMake
%QTDIR%\bin\qmake -recursive -tp vc

cd %~dp0\..\..
call Config\QMake\CopyVCProjToSubdir.js %COMPILER_EXT%

cd %~dp0\
