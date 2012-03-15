cd Temp

echo Generating VC8 projects...
set QMAKESPEC=%QTDIR%/mkspecs/win32-msvc2005
cd Impl\QMake
qmake -recursive -tp vc AcfAll.pro
cd ..\..

call ..\..\Config\QMake\CopyVCProjToSubdir.js VC8

echo Generating VC9 projects...
set QMAKESPEC=%QTDIR%/mkspecs/win32-msvc2008
cd Impl\QMake
qmake -recursive -tp vc AcfAll.pro
cd ..\..

call ..\..\Config\QMake\CopyVCProjToSubdir.js VC9

cd ..
