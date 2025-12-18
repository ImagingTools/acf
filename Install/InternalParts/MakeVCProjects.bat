echo Generating %COMPILER_EXT% projects...
cd Build/QMake
qmake -recursive -tp vc *All.pro
cd ../..

call %~dp0\..\..\Config\QMake\CopyVCProjToSubdir.js %COMPILER_EXT% -replace%QTDIR%=$(QTDIR) -replace%CD%\=$(RelativePath)\

