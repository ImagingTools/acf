echo off

if not exist Temp goto DelTempSkip
echo Removing old temporary files...
rmdir /Q /S Temp
:DelTempSkip

if not exist TempExcl goto DelTempExclSkip
echo Removing old temporary scripts...
rmdir /Q /S TempExcl
:DelTempExclSkip

mkdir Temp
mkdir TempExcl

set COMPILER_EXT=VC8

rem call Rebuild.bat

rem call Prepare_ACF.bat

call Create_TechnicalDoc.bat

rem call Create_VC_LGPL.bat

rem call Compile_Setup.bat

echo Installation done

pause

echo Removing temporary files...
rmdir /Q /S Temp
rmdir /Q /S TempExcl

