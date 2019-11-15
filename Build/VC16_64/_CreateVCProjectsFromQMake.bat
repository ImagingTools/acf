set COMPILER_EXT=VC16_64

set QMAKESPEC=%QTDIR%\mkspecs\win32-msvc

echo Generating %COMPILER_EXT% projects...

cd %~dp0\..\..\Build\QMake
%QTDIR%\bin\qmake -recursive -tp vc

cd %~dp0\..\..\Docs\Tutorial\Build\QMake
%QTDIR%\bin\qmake -recursive -tp vc

cd %~dp0\..\..
call Config\QMake\CopyVCProjToSubdir.js %COMPILER_EXT% -replace%QTDIR%=$(QTDIR) -replace%ACF_TOOLS_BIN%\=$(ACF_TOOLS_BIN)\ -replace%CD%\=$(RelativePath)\

cd %~dp0\

pause

