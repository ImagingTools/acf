echo off

echo Building ACF

cd ../Impl/VC8
vcbuild /r AcfAll.sln $All

echo Building ACF done

echo Building ACF Tutorials

cd ../../Docs/Tutorial/Appl/VC8
vcbuild /r TutorialAcf.sln $All
echo Building ACF Tutorials done

cd ../../../../Install

echo Preparing ACF framework

copy /Y %QTDIR%\bin\*.dll ..\Bin\ReleaseVC8
copy /Y %QTDIR%\bin\*.dll ..\Bin\DebugVC8
copy /Y ..\Bin\Info\*.xpc ..\Bin\ReleaseVC8
copy /Y ..\Bin\Info\*.xpc ..\Bin\DebugVC8

echo Preparing ACF framework done

