echo Generating VC10 projects...

cd ..\..
call Config\QMake\CopyProToSubdir.js VC10
cd Impl\VC10

pause

set QMAKESPEC=%QTDIR%/mkspecs/win32-msvc2010
qmake -recursive -tp vc AcfAll.pro

del /q Static.sln
del /q Component.sln
del /q Tool.sln

cd ..\..
call Config\QMake\RemoveProFromSubdir.js VC10
cd Impl\VC10

pause
