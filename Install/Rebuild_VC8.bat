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

