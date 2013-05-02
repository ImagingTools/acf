echo off

echo Building ACF

cd Build/%COMPILER_EXT%
vcbuild /r AcfAll.sln $All

echo Building ACF done

echo Building ACF Tutorials

cd ../Install/Docs/Tutorial/Build/%COMPILER_EXT%
vcbuild /r TutorialAcf.sln $All
echo Building ACF Tutorials done

cd ../../../..

